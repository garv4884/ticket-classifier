#pragma once

#include <memory>
#include <string>

#include "models/Ticket.h"

// Factory: turns a routed category into the matching concrete Ticket subclass.
// Adding a category means adding one Ticket subclass and one case here.
class TicketFactory {
public:
    std::unique_ptr<Ticket> create(Category category, std::string id, std::string subject,
                                   std::string description, Priority priority,
                                   SentimentResult sentiment) const;
};
