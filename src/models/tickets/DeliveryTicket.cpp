#include "models/tickets/DeliveryTicket.h"

#include "utils/TextUtils.h"

void DeliveryTicket::extractDetails() {
    static const std::regex orderPattern = text::labelledIdPattern("order");
    static const std::regex trackingPattern = text::labelledIdPattern("tracking|awb");

    if (auto order = text::firstMatch(fullText(), orderPattern)) addDetail("order_id", *order);
    if (auto tracking = text::firstMatch(fullText(), trackingPattern)) addDetail("tracking_id", *tracking);
}
