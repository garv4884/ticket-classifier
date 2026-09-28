#include "states/EscalatedState.h"

#include <memory>

#include "models/Ticket.h"
#include "states/ClosedState.h"

void EscalatedState::close(Ticket& ticket) {
    ticket.changeState(std::make_unique<ClosedState>());
}
