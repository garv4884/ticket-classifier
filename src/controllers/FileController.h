#pragma once

#include <filesystem>
#include <istream>
#include <vector>

#include "controllers/ITicketController.h"
#include "models/TicketRequest.h"

class TicketService;

// Batch entry: reads tickets from a text file in the format
//
//   SUBJECT: ...
//   DESCRIPTION: ...        (following lines without a key continue the field)
//   PRIORITY: P1
//   ---
//
// Lines starting with '#' are comments.
class FileController : public ITicketController {
public:
    FileController(TicketService& service, std::filesystem::path inputFile);

    void run() override;

    static std::vector<TicketRequest> parse(std::istream& in);

private:
    TicketService& service_;
    std::filesystem::path inputFile_;
};
