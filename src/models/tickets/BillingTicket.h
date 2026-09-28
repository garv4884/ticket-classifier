#pragma once

#include "models/Ticket.h"

// Extracts: invoice_id (invoice / transaction / txn), amount ($, Rs, INR, USD).
class BillingTicket : public Ticket {
public:
    using Ticket::Ticket;

    Category category() const override { return Category::BILLING; }
    void extractDetails() override;
};
