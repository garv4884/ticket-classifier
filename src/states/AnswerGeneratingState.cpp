#include "states/AnswerGeneratingState.h"

#include <memory>

#include "models/Ticket.h"
#include "states/RepliedState.h"

void AnswerGeneratingState::markReplied(Ticket& ticket) {
    ticket.changeState(std::make_unique<RepliedState>());
}
