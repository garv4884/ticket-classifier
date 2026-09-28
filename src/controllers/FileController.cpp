#include "controllers/FileController.h"

#include <fstream>
#include <iostream>
#include <string>

#include "controllers/ResultPrinter.h"
#include "services/TicketService.h"
#include "utils/TextUtils.h"

FileController::FileController(TicketService& service, std::filesystem::path inputFile)
    : service_(service), inputFile_(std::move(inputFile)) {}

void FileController::run() {
    std::ifstream in(inputFile_);
    if (!in) {
        std::cerr << "Could not open input file: " << inputFile_.string() << '\n';
        return;
    }

    const std::vector<TicketRequest> requests = parse(in);
    std::cout << "Processing " << requests.size() << " ticket(s) from " << inputFile_.string() << "\n";
    for (std::size_t i = 0; i < requests.size(); ++i) {
        std::cout << '\n' << std::string(70, '-') << "\nTicket " << i + 1 << ": ["
                  << requests[i].priority << "] " << requests[i].subject << '\n';
        printResult(std::cout, service_.submit(requests[i]));
    }
}

std::vector<TicketRequest> FileController::parse(std::istream& in) {
    std::vector<TicketRequest> requests;
    TicketRequest current;
    bool hasContent = false;
    std::string* lastField = nullptr;

    const auto flush = [&] {
        if (hasContent) requests.push_back(current);
        current = TicketRequest{};
        hasContent = false;
        lastField = nullptr;
    };

    std::string line;
    while (std::getline(in, line)) {
        const std::string trimmed = text::trim(line);
        if (trimmed.empty() || trimmed[0] == '#') continue;
        if (trimmed == "---") {
            flush();
            continue;
        }

        const auto colon = trimmed.find(':');
        const std::string key =
            colon == std::string::npos ? "" : text::toUpper(text::trim(trimmed.substr(0, colon)));

        if (key == "SUBJECT") {
            lastField = &current.subject;
        } else if (key == "DESCRIPTION") {
            lastField = &current.description;
        } else if (key == "PRIORITY") {
            lastField = &current.priority;
        } else {
            if (lastField) *lastField += " " + trimmed;  // continuation line
            continue;
        }
        *lastField = text::trim(trimmed.substr(colon + 1));
        hasContent = true;
    }
    flush();
    return requests;
}
