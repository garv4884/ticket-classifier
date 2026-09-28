#pragma once

#include "models/Ticket.h"

// Extracts: order_id.
class ReturnsTicket : public Ticket {
public:
    using Ticket::Ticket;

    Category category() const override { return Category::RETURNS; }
    void extractDetails() override;
};
