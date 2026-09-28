#include "states/ProcessingState.h"

#include <memory>

#include "models/Ticket.h"
#include "states/AnswerGeneratingState.h"

void ProcessingState::startAnswering(Ticket& ticket) {
    ticket.changeState(std::make_unique<AnswerGeneratingState>());
}
