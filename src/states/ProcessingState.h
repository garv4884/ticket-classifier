#pragma once

#include "states/TicketState.h"

class ProcessingState : public TicketState {
public:
    std::string name() const override { return "PROCESSING"; }
    void startAnswering(Ticket& ticket) override;
};
