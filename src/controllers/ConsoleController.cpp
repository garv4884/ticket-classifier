#include "controllers/ConsoleController.h"

#include <iostream>

#include "controllers/ResultPrinter.h"
#include "repository/ITicketRepository.h"
#include "services/TicketService.h"
#include "utils/TextUtils.h"

ConsoleController::ConsoleController(TicketService& service, const ITicketRepository& repository)
    : service_(service), repository_(repository) {}

void ConsoleController::run() {
    std::cout << "=== Customer Support Ticket Router ===\n";
    while (true) {
        std::cout << "\n1) Submit a ticket\n2) View tickets by category\n3) Exit\n";
        const std::string choice = text::trim(prompt("Choose: "));
        if (!std::cin) return;

        if (choice == "1") {
            submitTicket();
        } else if (choice == "2") {
            viewTickets();
        } else if (choice == "3") {
            return;
        } else {
            std::cout << "Please enter 1, 2 or 3.\n";
        }
    }
}

void ConsoleController::submitTicket() {
    TicketRequest request;
    request.subject = prompt("Subject     : ");
    request.description = prompt("Description : ");
    request.priority = prompt("Priority (P0 highest, P1, P2): ");
    std::cout << '\n';
    printResult(std::cout, service_.submit(request));
}

void ConsoleController::viewTickets() const {
    const auto& categories = allCategories();
    for (std::size_t i = 0; i < categories.size(); ++i) {
        std::cout << i + 1 << ") " << toString(categories[i]) << '\n';
    }

    std::size_t index = 0;
    try {
        index = std::stoul(prompt("Category number: "));
    } catch (const std::exception&) {
        index = 0;
    }
    if (index < 1 || index > categories.size()) {
        std::cout << "Unknown category.\n";
        return;
    }

    const std::string content = repository_.loadAll(categories[index - 1]);
    std::cout << '\n' << (content.empty() ? "(no tickets yet)\n" : content);
}

std::string ConsoleController::prompt(const std::string& label) {
    std::cout << label;
    std::string line;
    std::getline(std::cin, line);
    return line;
}
