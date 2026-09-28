#include "factories/TicketFactory.h"

#include <stdexcept>

#include "models/tickets/AccountTicket.h"
#include "models/tickets/BillingTicket.h"
#include "models/tickets/DeliveryTicket.h"
#include "models/tickets/GeneralTicket.h"
#include "models/tickets/ReturnsTicket.h"
#include "models/tickets/TechnicalTicket.h"

std::unique_ptr<Ticket> TicketFactory::create(Category category, std::string id, std::string subject,
                                              std::string description, Priority priority,
                                              SentimentResult sentiment) const {
    switch (category) {
        case Category::BILLING:
            return std::make_unique<BillingTicket>(std::move(id), std::move(subject),
                                                   std::move(description), priority, sentiment);
        case Category::TECHNICAL:
            return std::make_unique<TechnicalTicket>(std::move(id), std::move(subject),
                                                     std::move(description), priority, sentiment);
        case Category::DELIVERY:
            return std::make_unique<DeliveryTicket>(std::move(id), std::move(subject),
                                                    std::move(description), priority, sentiment);
        case Category::ACCOUNT:
            return std::make_unique<AccountTicket>(std::move(id), std::move(subject),
                                                   std::move(description), priority, sentiment);
        case Category::RETURNS:
            return std::make_unique<ReturnsTicket>(std::move(id), std::move(subject),
                                                   std::move(description), priority, sentiment);
        case Category::GENERAL:
            return std::make_unique<GeneralTicket>(std::move(id), std::move(subject),
                                                   std::move(description), priority, sentiment);
    }
    throw std::invalid_argument("Unknown ticket category");
}
