#pragma once

#include <string>

#include "models/Enums.h"

// What the idempotency store remembers about a processed ticket: enough to
// answer a duplicate submission with the original outcome.
struct TicketRecord {
    std::string ticketId;
    Category category = Category::GENERAL;
    Sentiment sentiment = Sentiment::NEUTRAL;
    int sentimentScore = 0;
    std::string state;
    std::string reply;
};
