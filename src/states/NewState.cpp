#include "states/NewState.h"

#include <memory>

#include "models/Ticket.h"
#include "states/ProcessingState.h"

void NewState::startProcessing(Ticket& ticket) {
    ticket.changeState(std::make_unique<ProcessingState>());
}
