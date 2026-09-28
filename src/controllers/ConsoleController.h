#pragma once

#include <string>

#include "controllers/ITicketController.h"

class TicketService;
class ITicketRepository;

// Interactive menu: submit tickets one by one or browse a category's file.
class ConsoleController : public ITicketController {
public:
    ConsoleController(TicketService& service, const ITicketRepository& repository);

    void run() override;

private:
    void submitTicket();
    void viewTickets() const;
    static std::string prompt(const std::string& label);

    TicketService& service_;
    const ITicketRepository& repository_;
};
