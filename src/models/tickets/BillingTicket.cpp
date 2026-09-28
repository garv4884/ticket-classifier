#include "models/tickets/BillingTicket.h"

#include "utils/TextUtils.h"

void BillingTicket::extractDetails() {
    static const std::regex invoicePattern = text::labelledIdPattern("invoice|transaction|txn");
    static const std::regex amountPattern(R"((?:\$|\b(?:usd|rs\.?|inr)\s?)\d+(?:\.\d{1,2})?)",
                                          std::regex::icase);

    if (auto invoice = text::firstMatch(fullText(), invoicePattern)) addDetail("invoice_id", *invoice);
    if (auto amount = text::firstMatch(fullText(), amountPattern, 0)) addDetail("amount", *amount);
}
