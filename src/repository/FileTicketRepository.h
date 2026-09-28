#pragma once

#include <filesystem>

#include "repository/ITicketRepository.h"

// One text file per category (billing.txt, technical.txt, ...) inside the
// output directory; each ticket is appended as a readable block.
class FileTicketRepository : public ITicketRepository {
public:
    explicit FileTicketRepository(std::filesystem::path outputDir);

    void save(const Ticket& ticket) override;
    std::string loadAll(Category category) const override;

private:
    std::filesystem::path fileFor(Category category) const;

    std::filesystem::path outputDir_;
};
