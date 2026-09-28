#pragma once

#include "models/Ticket.h"

// Extracts: email.
class AccountTicket : public Ticket {
public:
    using Ticket::Ticket;

    Category category() const override { return Category::ACCOUNT; }
    void extractDetails() override;
};
