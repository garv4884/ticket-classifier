#include "models/tickets/ReturnsTicket.h"

#include "utils/TextUtils.h"

void ReturnsTicket::extractDetails() {
    static const std::regex orderPattern = text::labelledIdPattern("order");

    if (auto order = text::firstMatch(fullText(), orderPattern)) addDetail("order_id", *order);
}
