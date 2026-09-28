#pragma once

#include <string>
#include <vector>

#include "controllers/ITicketController.h"
#include "models/TicketRequest.h"

class TicketService;

// Showcase entry point: pushes a prepared set of tickets through the full
// pipeline - every category, every sentiment level, all priorities, an
// escalation, duplicates (idempotency) and invalid input - and prints a
// summary table at the end.
class DemoController : public ITicketController {
public:
    explicit DemoController(TicketService& service);

    void run() override;

private:
    struct DemoCase {
        std::string whatItShows;
        TicketRequest request;
    };

    static std::vector<DemoCase> cases();

    TicketService& service_;
};
