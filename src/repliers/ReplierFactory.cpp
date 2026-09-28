#include "repliers/ReplierFactory.h"

#include <stdexcept>

#include "repliers/AccountReplier.h"
#include "repliers/BillingReplier.h"
#include "repliers/DeliveryReplier.h"
#include "repliers/GeneralReplier.h"
#include "repliers/ReturnsReplier.h"
#include "repliers/TechnicalReplier.h"

ReplierFactory::ReplierFactory() {
    repliers_.emplace(Category::BILLING, std::make_unique<BillingReplier>());
    repliers_.emplace(Category::TECHNICAL, std::make_unique<TechnicalReplier>());
    repliers_.emplace(Category::DELIVERY, std::make_unique<DeliveryReplier>());
    repliers_.emplace(Category::ACCOUNT, std::make_unique<AccountReplier>());
    repliers_.emplace(Category::RETURNS, std::make_unique<ReturnsReplier>());
    repliers_.emplace(Category::GENERAL, std::make_unique<GeneralReplier>());
}

const IReplier& ReplierFactory::forCategory(Category category) const {
    const auto it = repliers_.find(category);
    if (it == repliers_.end()) {
        throw std::invalid_argument("No replier registered for " + toString(category));
    }
    return *it->second;
}
