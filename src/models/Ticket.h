#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "models/AnalysisResults.h"
#include "models/Enums.h"

class TicketState;

// Base class for every ticket. Concrete subclasses (BillingTicket, ...) are
// created by TicketFactory once the ticket has been routed, and each knows how
// to pull its own category-specific details (order id, invoice id, ...) out of
// the text.
//
// A ticket is also the context of the State pattern: lifecycle events are
// delegated to the current TicketState, which decides whether they are legal.
class Ticket {
public:
    Ticket(std::string id, std::string subject, std::string description,
           Priority priority, SentimentResult sentiment);
    virtual ~Ticket();

    Ticket(const Ticket&) = delete;
    Ticket& operator=(const Ticket&) = delete;

    virtual Category category() const = 0;
    virtual void extractDetails() = 0;

    // Lifecycle events, delegated to the current state.
    void startProcessing();
    void startAnswering();
    void markReplied();
    void close();
    void escalate();

    // Called only by TicketState implementations.
    void changeState(std::unique_ptr<TicketState> next);

    std::string stateName() const;
    const std::vector<std::string>& stateHistory() const { return stateHistory_; }

    const std::string& id() const { return id_; }
    const std::string& subject() const { return subject_; }
    const std::string& description() const { return description_; }
    std::string fullText() const { return subject_ + " " + description_; }
    Priority priority() const { return priority_; }
    const SentimentResult& sentiment() const { return sentiment_; }
    const std::string& createdAt() const { return createdAt_; }

    const std::map<std::string, std::string>& details() const { return details_; }
    std::optional<std::string> detail(const std::string& key) const;

    const std::string& reply() const { return reply_; }
    void setReply(std::string reply) { reply_ = std::move(reply); }

    bool escalationRequired() const { return escalationRequired_; }
    void requireEscalation() { escalationRequired_ = true; }

protected:
    void addDetail(const std::string& key, const std::string& value) { details_[key] = value; }

private:
    std::string id_;
    std::string subject_;
    std::string description_;
    Priority priority_;
    SentimentResult sentiment_;
    std::string createdAt_;
    std::map<std::string, std::string> details_;
    std::string reply_;
    bool escalationRequired_ = false;
    std::unique_ptr<TicketState> state_;
    std::vector<std::string> stateHistory_;
};
