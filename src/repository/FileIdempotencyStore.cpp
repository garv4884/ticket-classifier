#include "repository/FileIdempotencyStore.h"

#include <fstream>
#include <stdexcept>
#include <vector>

#include "utils/TextUtils.h"

namespace {
constexpr std::size_t kFieldCount = 7;
}

FileIdempotencyStore::FileIdempotencyStore(std::filesystem::path indexFile)
    : indexFile_(std::move(indexFile)) {
    if (indexFile_.has_parent_path()) std::filesystem::create_directories(indexFile_.parent_path());
    load();
}

std::optional<TicketRecord> FileIdempotencyStore::find(const std::string& key) const {
    const auto it = records_.find(key);
    if (it == records_.end()) return std::nullopt;
    return it->second;
}

void FileIdempotencyStore::remember(const std::string& key, const TicketRecord& record) {
    std::ofstream out(indexFile_, std::ios::app);
    if (!out) throw std::runtime_error("Could not open " + indexFile_.string());
    out << text::escapeField(key) << '\t'
        << text::escapeField(record.ticketId) << '\t'
        << toString(record.category) << '\t'
        << toString(record.sentiment) << '\t'
        << record.sentimentScore << '\t'
        << text::escapeField(record.state) << '\t'
        << text::escapeField(record.reply) << '\n';
    records_[key] = record;
}

void FileIdempotencyStore::load() {
    std::ifstream in(indexFile_);
    std::string line;
    while (std::getline(in, line)) {
        const std::vector<std::string> fields = text::split(line, '\t');
        if (fields.size() != kFieldCount) continue;  // skip malformed lines

        const auto category = parseCategory(fields[2]);
        const auto sentiment = parseSentiment(fields[3]);
        if (!category || !sentiment) continue;

        int score = 0;
        try {
            score = std::stoi(fields[4]);
        } catch (const std::exception&) {
            continue;
        }
        records_[text::unescapeField(fields[0])] =
            TicketRecord{text::unescapeField(fields[1]), *category, *sentiment, score,
                         text::unescapeField(fields[5]), text::unescapeField(fields[6])};
    }
}
