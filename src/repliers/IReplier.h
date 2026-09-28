#pragma once

#include <string>

class Ticket;

// Strategy: writes the pre-written style reply for one category.
class IReplier {
public:
    virtual ~IReplier() = default;

    virtual std::string generate(const Ticket& ticket) const = 0;
};
