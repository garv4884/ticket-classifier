#pragma once

#include <string>

#include "models/SubmissionResult.h"
#include "models/TicketRequest.h"

class IClassifier;
class ISentimentAnalyzer;
class TicketFactory;
class ReplierFactory;
class ITicketRepository;
class IIdempotencyStore;
class IdGenerator;
class Ticket;
struct TicketRecord;

// Orchestrates one submission end to end:
//   validate -> idempotency check -> classify + sentiment -> create ticket
//   -> PROCESSING (extract details, escalation check)
//   -> ANSWER_GENERATING (reply) -> REPLIED -> CLOSED / ESCALATED -> persist
// Depends only on abstractions for the swappable parts (DIP).
class TicketService {
public:
    TicketService(const IClassifier& classifier, const ISentimentAnalyzer& sentimentAnalyzer,
                  const TicketFactory& ticketFactory, const ReplierFactory& replierFactory,
                  ITicketRepository& repository, IIdempotencyStore& idempotencyStore,
                  IdGenerator& idGenerator);

    static constexpr std::size_t kMaxSubjectLength = 256;
    static constexpr std::size_t kMaxDescriptionLength = 8192;

    SubmissionResult submit(const TicketRequest& request);

private:
    static bool needsEscalation(const Ticket& ticket);
    static SubmissionResult rejected(const std::string& reason);
    static SubmissionResult duplicateOf(const TicketRecord& record);

    const IClassifier& classifier_;
    const ISentimentAnalyzer& sentimentAnalyzer_;
    const TicketFactory& ticketFactory_;
    const ReplierFactory& replierFactory_;
    ITicketRepository& repository_;
    IIdempotencyStore& idempotencyStore_;
    IdGenerator& idGenerator_;
};
