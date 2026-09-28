#pragma once

#include <string>

class Ticket;

// State pattern. Legal lifecycle:
//
//   NEW -> PROCESSING -> ANSWER_GENERATING -> REPLIED -> CLOSED
//                                                  \-> ESCALATED -> CLOSED
//
// Every event is illegal by default; each concrete state overrides only the
// events it allows and moves the ticket to the next state. States guard
// transitions only - the actual work is done by TicketService.
class TicketState {
public:
    virtual ~TicketState() = default;

    virtual std::string name() const = 0;

    virtual void startProcessing(Ticket& ticket);
    virtual void startAnswering(Ticket& ticket);
    virtual void markReplied(Ticket& ticket);
    virtual void close(Ticket& ticket);
    virtual void escalate(Ticket& ticket);

protected:
    [[noreturn]] void reject(const std::string& action) const;
};
