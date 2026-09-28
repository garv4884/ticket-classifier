#include "controllers/DemoController.h"

#include <iomanip>
#include <iostream>

#include "controllers/ResultPrinter.h"
#include "services/TicketService.h"

DemoController::DemoController(TicketService& service) : service_(service) {}

std::vector<DemoController::DemoCase> DemoController::cases() {
    const TicketRequest billingAngry{
        "Charged TWICE for my subscription!!!",
        "This is absolutely unacceptable. I was charged twice for invoice INV-2291 and $49.99 was "
        "deducted two times from my credit card. Worst service ever, fix it NOW!!!",
        "P0"};
    const TicketRequest deliveryLate{
        "Package not delivered yet",
        "My order #48213 was supposed to arrive last Friday but it is still not delivered. "
        "Tracking number TRK-99812 has not updated in 4 days. Very disappointed.",
        "P1"};

    return {
        {"Billing | angry customer | P0 -> escalated", billingAngry},
        {"Technical | frustrated | P1 | error code + platform extracted",
         {"App keeps crashing after update",
          "Since the latest update the app crashes every time I open it on my Android phone. It "
          "shows error code E-502. Really frustrating.",
          "P1"}},
        {"Delivery | disappointed | P1 | order + tracking id extracted", deliveryLate},
        {"Account | neutral | P2 | email extracted",
         {"Forgot my password",
          "Hi, I forgot my password and the reset email is not arriving at jane.doe@example.com. "
          "Could you help me log in?",
          "P2"}},
        {"Returns | damaged item | P2",
         {"Received a damaged blender",
          "Unfortunately the blender from order ORD-7731 arrived damaged and the jar is cracked. I "
          "would like a replacement please.",
          "P2"}},
        {"General | positive feedback | P2",
         {"Loving the new dashboard",
          "Just wanted to say thanks, the new dashboard is great and really helpful. One "
          "suggestion: a dark mode would be amazing!",
          "p2"}},
        {"Account security | furious | P0 -> escalated",
         {"Account HACKED - unauthorized orders placed",
          "Someone hacked my account and placed orders I never made! This is a disaster and I am "
          "furious. Please lock it immediately!!!",
          "P0"}},
        {"Technical | neutral | P2 | performance sub-topic",
         {"Website is slow to load", "Pages on the website take more than 30 seconds to load in Chrome.",
          "P2"}},
        {"Returns vs Billing overlap on 'refund' -> Returns wins on score",
         {"Refund for returned shoes",
          "I returned the shoes from order ORD-5521 two weeks ago but still have not received my "
          "refund of $80.",
          "P1"}},
        {"Delivery | negation handling ('not happy', 'not good')",
         {"Not happy with my delivery", "My parcel is delayed again and nobody has updated me. Not good.",
          "P1"}},
        {"Idempotency: case 3 again, different casing/punctuation/priority",
         {"PACKAGE NOT DELIVERED YET!!",
          "My order 48213 was supposed to arrive last Friday, but it is STILL not delivered.  "
          "Tracking number TRK-99812 has not updated in 4 days... very disappointed!",
          "P0"}},
        {"Idempotency: exact resubmission of case 1", billingAngry},
        {"Validation: invalid priority", {"Need help", "Where can I find my invoices?", "P5"}},
        {"Validation: empty description", {"Refund please", "   ", "P1"}},
    };
}

void DemoController::run() {
    std::cout << "=== Customer Support Ticket Router - DEMO ===\n"
              << "Tip: tickets and the idempotency index persist in the data folder; run with\n"
              << "--reset to start clean, otherwise repeated runs show up as DUPLICATE.\n";

    struct Row {
        std::size_t number;
        SubmissionResult result;
    };
    std::vector<Row> rows;

    const std::vector<DemoCase> demoCases = cases();
    for (std::size_t i = 0; i < demoCases.size(); ++i) {
        const DemoCase& demoCase = demoCases[i];
        std::cout << '\n' << std::string(70, '=') << "\nCase " << i + 1 << ": " << demoCase.whatItShows
                  << "\n  Input  -> [" << demoCase.request.priority << "] " << demoCase.request.subject
                  << '\n' << std::string(70, '-') << '\n';

        SubmissionResult result = service_.submit(demoCase.request);
        printResult(std::cout, result);
        rows.push_back({i + 1, std::move(result)});
    }

    std::cout << '\n' << std::string(70, '=') << "\nSUMMARY\n" << std::string(70, '=') << '\n'
              << std::left << std::setw(6) << "Case" << std::setw(11) << "Status" << std::setw(10) << "Ticket"
              << std::setw(11) << "Category" << std::setw(10) << "Sentiment" << "State\n";
    for (const Row& row : rows) {
        const SubmissionResult& r = row.result;
        const bool rejected = r.status == SubmissionStatus::REJECTED;
        std::cout << std::setw(6) << row.number << std::setw(11) << toString(r.status)
                  << std::setw(10) << (rejected ? "-" : r.ticketId)
                  << std::setw(11) << (rejected ? "-" : toString(r.category))
                  << std::setw(10) << (rejected ? "-" : toString(r.sentiment))
                  << (rejected ? "-" : r.finalState) << '\n';
    }
}
