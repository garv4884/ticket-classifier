#include "models/tickets/AccountTicket.h"

#include "utils/TextUtils.h"

void AccountTicket::extractDetails() {
    static const std::regex emailPattern(R"([A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,})");

    if (auto email = text::firstMatch(fullText(), emailPattern, 0)) addDetail("email", *email);
}
