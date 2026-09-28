#include "states/TicketState.h"

#include <stdexcept>

void TicketState::startProcessing(Ticket&) { reject("start processing"); }
void TicketState::startAnswering(Ticket&) { reject("start answering"); }
void TicketState::markReplied(Ticket&) { reject("mark as replied"); }
void TicketState::close(Ticket&) { reject("close"); }
void TicketState::escalate(Ticket&) { reject("escalate"); }

void TicketState::reject(const std::string& action) const {
    throw std::logic_error("Illegal transition: cannot " + action + " a ticket in state " + name());
}
