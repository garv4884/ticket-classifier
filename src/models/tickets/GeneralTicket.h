#pragma once

#include "models/Ticket.h"

// Fallback queue for tickets that match no category clearly. Nothing to extract.
class GeneralTicket : public Ticket {
public:
    using Ticket::Ticket;

    Category category() const override { return Category::GENERAL; }
    void extractDetails() override {}
};
