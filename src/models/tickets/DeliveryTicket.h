#pragma once

#include "models/Ticket.h"

// Extracts: order_id, tracking_id.
class DeliveryTicket : public Ticket {
public:
    using Ticket::Ticket;

    Category category() const override { return Category::DELIVERY; }
    void extractDetails() override;
};
