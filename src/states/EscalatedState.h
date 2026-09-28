#pragma once

#include "states/TicketState.h"

// Handed to a senior support lead; closes once they resolve it.
class EscalatedState : public TicketState {
public:
    std::string name() const override { return "ESCALATED"; }
    void close(Ticket& ticket) override;
};
