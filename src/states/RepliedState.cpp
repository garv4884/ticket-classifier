#include "states/RepliedState.h"

#include <memory>

#include "models/Ticket.h"
#include "states/ClosedState.h"
#include "states/EscalatedState.h"

void RepliedState::close(Ticket& ticket) {
    ticket.changeState(std::make_unique<ClosedState>());
}

void RepliedState::escalate(Ticket& ticket) {
    ticket.changeState(std::make_unique<EscalatedState>());
}
