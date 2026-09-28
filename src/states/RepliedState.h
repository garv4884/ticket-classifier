#pragma once

#include "states/TicketState.h"

class RepliedState : public TicketState {
public:
    std::string name() const override { return "REPLIED"; }
    void close(Ticket& ticket) override;
    void escalate(Ticket& ticket) override;
};
