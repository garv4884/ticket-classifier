#pragma once

#include "models/Ticket.h"

// Extracts: error_code, platform (Android, iOS, Windows, Chrome, ...).
class TechnicalTicket : public Ticket {
public:
    using Ticket::Ticket;

    Category category() const override { return Category::TECHNICAL; }
    void extractDetails() override;
};
