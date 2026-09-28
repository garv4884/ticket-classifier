#pragma once

#include <filesystem>
#include <unordered_map>

#include "repository/IIdempotencyStore.h"

// Index file with one tab-separated line per processed ticket:
//   key  ticketId  category  sentiment  score  state  reply(escaped)
// Loaded into memory on start-up so duplicates are caught across runs.
class FileIdempotencyStore : public IIdempotencyStore {
public:
    explicit FileIdempotencyStore(std::filesystem::path indexFile);

    std::optional<TicketRecord> find(const std::string& key) const override;
    void remember(const std::string& key, const TicketRecord& record) override;
    std::size_t size() const override { return records_.size(); }

private:
    void load();

    std::filesystem::path indexFile_;
    std::unordered_map<std::string, TicketRecord> records_;
};
