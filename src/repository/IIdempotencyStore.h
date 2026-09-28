#pragma once

#include <cstddef>
#include <optional>
#include <string>

#include "models/TicketRecord.h"

// Remembers which idempotency keys were already processed, and with what
// outcome, so a repeated submission returns the original ticket instead of
// creating a copy.
class IIdempotencyStore {
public:
    virtual ~IIdempotencyStore() = default;

    virtual std::optional<TicketRecord> find(const std::string& key) const = 0;
    virtual void remember(const std::string& key, const TicketRecord& record) = 0;
    virtual std::size_t size() const = 0;
};
