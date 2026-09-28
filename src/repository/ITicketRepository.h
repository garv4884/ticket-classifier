#pragma once

#include <string>

#include "models/Enums.h"

class Ticket;

class ITicketRepository {
public:
    virtual ~ITicketRepository() = default;

    // Stores the ticket in its category's destination.
    virtual void save(const Ticket& ticket) = 0;

    // Everything stored for a category, as human-readable text.
    virtual std::string loadAll(Category category) const = 0;
};
