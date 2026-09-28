#pragma once

#include "states/TicketState.h"

class AnswerGeneratingState : public TicketState {
public:
    std::string name() const override { return "ANSWER_GENERATING"; }
    void markReplied(Ticket& ticket) override;
};
