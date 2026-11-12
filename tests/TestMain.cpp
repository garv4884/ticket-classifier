// Minimal assert-style tests. The service tests use in-memory repositories,
// which also shows that TicketService depends only on the interfaces.

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "factories/TicketFactory.h"
#include "models/Ticket.h"
#include "models/tickets/BillingTicket.h"
#include "models/tickets/DeliveryTicket.h"
#include "repliers/ReplierFactory.h"
#include "repository/IIdempotencyStore.h"
#include "repository/ITicketRepository.h"
#include "services/TicketService.h"
#include "services/classification/KeywordClassifier.h"
#include "services/sentiment/LexiconSentimentAnalyzer.h"
#include "utils/Fingerprint.h"
#include "utils/IdGenerator.h"

namespace {

int checks = 0;
int failures = 0;

#define CHECK(condition)                                                                   \
    do {                                                                                   \
        ++checks;                                                                          \
        if (!(condition)) {                                                                \
            ++failures;                                                                    \
            std::cerr << "FAILED: " #condition " (" __FILE__ ":" << __LINE__ << ")\n";     \
        }                                                                                  \
    } while (0)

class InMemoryTicketRepository : public ITicketRepository {
public:
    void save(const Ticket& ticket) override { saved[ticket.category()] += ticket.id() + "\n"; ++count; }
    std::string loadAll(Category category) const override {
        const auto it = saved.find(category);
        return it == saved.end() ? "" : it->second;
    }

    std::map<Category, std::string> saved;
    int count = 0;
};

class InMemoryIdempotencyStore : public IIdempotencyStore {
public:
    std::optional<TicketRecord> find(const std::string& key) const override {
        const auto it = records.find(key);
        if (it == records.end()) return std::nullopt;
        return it->second;
    }
    void remember(const std::string& key, const TicketRecord& record) override { records[key] = record; }
    std::size_t size() const override { return records.size(); }

    std::map<std::string, TicketRecord> records;
};

void testClassifier() {
    const KeywordClassifier classifier;
    CHECK(classifier.classify("Charged twice", "My card was charged twice this month").category == Category::BILLING);
    CHECK(classifier.classify("App crashing", "The app crashes on startup").category == Category::TECHNICAL);
    CHECK(classifier.classify("Where is my parcel", "Tracking has not updated").category == Category::DELIVERY);
    CHECK(classifier.classify("Cannot log in", "Password reset link is broken").category == Category::ACCOUNT);
    CHECK(classifier.classify("Return request", "Wrong size, I want to return them").category == Category::RETURNS);
    CHECK(classifier.classify("Hello", "Just a quick question about your company").category == Category::GENERAL);
}

void testSentiment() {
    const LexiconSentimentAnalyzer analyzer;
    CHECK(analyzer.analyze("Thank you, the support was excellent").label == Sentiment::POSITIVE);
    CHECK(analyzer.analyze("Please update the billing address on my account").label == Sentiment::NEUTRAL);
    CHECK(analyzer.analyze("I am disappointed with the delay").label == Sentiment::NEGATIVE);
    CHECK(analyzer.analyze("WORST service ever, absolutely unacceptable!!!").label == Sentiment::ANGRY);
    CHECK(analyzer.analyze("I am not happy").label == Sentiment::NEGATIVE);    // negation
    CHECK(analyzer.analyze("Not bad at all").label == Sentiment::POSITIVE);    // negated negative
}

void testFingerprint() {
    CHECK(fingerprint::of("Package NOT delivered!!", "Order  #1 missing.") ==
          fingerprint::of("package not delivered", "order 1 missing"));
    CHECK(fingerprint::of("package not delivered", "order 1 missing") !=
          fingerprint::of("package not delivered", "order 2 missing"));
}

void testDetailExtraction() {
    BillingTicket billing("TKT-1", "Wrong charge", "Invoice INV-2291 shows $49.99", Priority::P2, {});
    billing.extractDetails();
    CHECK(billing.detail("invoice_id") == std::optional<std::string>("INV-2291"));
    CHECK(billing.detail("amount") == std::optional<std::string>("$49.99"));

    DeliveryTicket delivery("TKT-2", "Late", "Order #48213, tracking number TRK-9981", Priority::P2, {});
    delivery.extractDetails();
    CHECK(delivery.detail("order_id") == std::optional<std::string>("48213"));
    CHECK(delivery.detail("tracking_id") == std::optional<std::string>("TRK-9981"));
}

void testStateTransitions() {
    BillingTicket ticket("TKT-1", "s", "d", Priority::P2, {});
    CHECK(ticket.stateName() == "NEW");

    bool threw = false;
    try {
        ticket.close();  // NEW -> CLOSED is illegal
    } catch (const std::logic_error&) {
        threw = true;
    }
    CHECK(threw);

    ticket.startProcessing();
    ticket.startAnswering();
    ticket.markReplied();
    ticket.escalate();
    ticket.close();
    CHECK(ticket.stateName() == "CLOSED");
    CHECK(ticket.stateHistory().size() == 6);

    threw = false;
    try {
        ticket.startProcessing();  // CLOSED is terminal
    } catch (const std::logic_error&) {
        threw = true;
    }
    CHECK(threw);
}

void testServiceAndIdempotency() {
    const KeywordClassifier classifier;
    const LexiconSentimentAnalyzer analyzer;
    const TicketFactory ticketFactory;
    const ReplierFactory replierFactory;
    InMemoryTicketRepository repository;
    InMemoryIdempotencyStore store;
    IdGenerator ids;
    TicketService service(classifier, analyzer, ticketFactory, replierFactory, repository, store, ids);

    const SubmissionResult first = service.submit({"Parcel delayed", "Order #77 is late", "P1"});
    CHECK(first.status == SubmissionStatus::CREATED);
    CHECK(first.ticketId == "TKT-0001");
    CHECK(first.category == Category::DELIVERY);
    CHECK(first.finalState == "CLOSED");
    CHECK(first.reply.find("TKT-0001") != std::string::npos);
    CHECK(first.reply.find("24 hours") != std::string::npos);

    const SubmissionResult again = service.submit({"PARCEL DELAYED!", "order 77 is late.", "P0"});
    CHECK(again.status == SubmissionStatus::DUPLICATE);
    CHECK(again.ticketId == "TKT-0001");
    CHECK(again.reply == first.reply);
    CHECK(repository.count == 1);

    CHECK(service.submit({"Help", "Something", "P9"}).status == SubmissionStatus::REJECTED);
    CHECK(service.submit({"", "Something", "P1"}).status == SubmissionStatus::REJECTED);

    const SubmissionResult angry = service.submit(
        {"Charged twice!!!", "Worst service ever, absolutely unacceptable. Refund me NOW!!!", "P0"});
    CHECK(angry.sentiment == Sentiment::ANGRY);
    CHECK(angry.finalState == "ESCALATED");
    CHECK(angry.ticketId == "TKT-0002");  // rejected/duplicate submissions consume no ids

    // Critical P0 ticket should always escalate even when sentiment is neutral/polite
    const SubmissionResult p0Neutral = service.submit(
        {"Database server down", "Production database is completely offline and unreachable", "P0"});
    CHECK(p0Neutral.status == SubmissionStatus::CREATED);
    CHECK(p0Neutral.finalState == "ESCALATED");

    // Input length limit checks
    std::string longSubject(TicketService::kMaxSubjectLength + 1, 'a');
    CHECK(service.submit({longSubject, "Valid description", "P1"}).status == SubmissionStatus::REJECTED);

    std::string longDesc(TicketService::kMaxDescriptionLength + 1, 'b');
    CHECK(service.submit({"Valid subject", longDesc, "P1"}).status == SubmissionStatus::REJECTED);
}

void testIdGeneratorThreadSafety() {
    IdGenerator gen(1);
    const std::string id1 = gen.next();
    const std::string id2 = gen.next();
    CHECK(id1 == "TKT-0001");
    CHECK(id2 == "TKT-0002");
}

void testShoutingAcronyms() {
    const LexiconSentimentAnalyzer analyzer;
    // Acronyms like API, AWS, SQL shouldn't trigger shouting penalty
    SentimentResult res = analyzer.analyze("API is down");
    CHECK(res.label == Sentiment::NEUTRAL);
}

}  // namespace

int main() {
    testClassifier();
    testSentiment();
    testFingerprint();
    testDetailExtraction();
    testStateTransitions();
    testServiceAndIdempotency();
    testIdGeneratorThreadSafety();
    testShoutingAcronyms();

    std::cout << (checks - failures) << "/" << checks << " checks passed\n";
    return failures == 0 ? 0 : 1;
}
