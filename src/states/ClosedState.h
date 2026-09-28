#pragma once

#include "states/TicketState.h"

// Terminal state: every event is rejected.
class ClosedState : public TicketState {
public:
    std::string name() const override { return "CLOSED"; }
};
