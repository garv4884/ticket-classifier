#include "services/TicketService.h"

#include <memory>
#include <optional>

#include "factories/TicketFactory.h"
#include "models/Ticket.h"
#include "models/TicketRecord.h"
#include "repliers/ReplierFactory.h"
#include "repository/IIdempotencyStore.h"
#include "repository/ITicketRepository.h"
#include "services/classification/IClassifier.h"
#include "services/sentiment/ISentimentAnalyzer.h"
#include "utils/Fingerprint.h"
#include "utils/IdGenerator.h"
#include "utils/TextUtils.h"

TicketService::TicketService(const IClassifier& classifier, const ISentimentAnalyzer& sentimentAnalyzer,
                             const TicketFactory& ticketFactory, const ReplierFactory& replierFactory,
                             ITicketRepository& repository, IIdempotencyStore& idempotencyStore,
                             IdGenerator& idGenerator)
    : classifier_(classifier),
      sentimentAnalyzer_(sentimentAnalyzer),
      ticketFactory_(ticketFactory),
      replierFactory_(replierFactory),
      repository_(repository),
      idempotencyStore_(idempotencyStore),
      idGenerator_(idGenerator) {}

SubmissionResult TicketService::submit(const TicketRequest& request) {
    const std::string subject = text::trim(request.subject);
    const std::string description = text::trim(request.description);
    if (subject.empty()) return rejected("Subject must not be empty.");
    if (description.empty()) return rejected("Description must not be empty.");
    const std::optional<Priority> priority = parsePriority(request.priority);
    if (!priority) return rejected("Invalid priority '" + request.priority + "'. Use P0, P1 or P2.");

    const std::string key = fingerprint::of(subject, description);
    if (std::optional<TicketRecord> existing = idempotencyStore_.find(key)) return duplicateOf(*existing);

    const ClassificationResult route = classifier_.classify(subject, description);
    const SentimentResult sentiment = sentimentAnalyzer_.analyze(subject + " " + description);

    std::unique_ptr<Ticket> ticket = ticketFactory_.create(route.category, idGenerator_.next(), subject,
                                                           description, *priority, sentiment);

    ticket->startProcessing();
    ticket->extractDetails();
    if (needsEscalation(*ticket)) ticket->requireEscalation();

    ticket->startAnswering();
    ticket->setReply(replierFactory_.forCategory(ticket->category()).generate(*ticket));
    ticket->markReplied();

    if (ticket->escalationRequired()) {
        ticket->escalate();
    } else {
        ticket->close();
    }

    repository_.save(*ticket);
    idempotencyStore_.remember(key, TicketRecord{ticket->id(), ticket->category(), sentiment.label,
                                                 sentiment.score, ticket->stateName(), ticket->reply()});

    SubmissionResult result;
    result.status = SubmissionStatus::CREATED;
    result.ticketId = ticket->id();
    result.priority = ticket->priority();
    result.category = ticket->category();
    result.sentiment = sentiment.label;
    result.sentimentScore = sentiment.score;
    result.matchedKeywords = route.matchedKeywords;
    result.details = ticket->details();
    result.lifecycle = ticket->stateHistory();
    result.finalState = ticket->stateName();
    result.reply = ticket->reply();
    return result;
}

bool TicketService::needsEscalation(const Ticket& ticket) {
    return ticket.priority() == Priority::P0 && ticket.sentiment().label == Sentiment::ANGRY;
}

SubmissionResult TicketService::rejected(const std::string& reason) {
    SubmissionResult result;
    result.status = SubmissionStatus::REJECTED;
    result.message = reason;
    return result;
}

SubmissionResult TicketService::duplicateOf(const TicketRecord& record) {
    SubmissionResult result;
    result.status = SubmissionStatus::DUPLICATE;
    result.message = "Same ticket was already submitted as " + record.ticketId +
                     "; no copy created, returning the original response.";
    result.ticketId = record.ticketId;
    result.category = record.category;
    result.sentiment = record.sentiment;
    result.sentimentScore = record.sentimentScore;
    result.finalState = record.state;
    result.reply = record.reply;
    return result;
}
