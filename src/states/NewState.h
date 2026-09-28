#pragma once

#include "states/TicketState.h"

class NewState : public TicketState {
public:
    std::string name() const override { return "NEW"; }
    void startProcessing(Ticket& ticket) override;
};
