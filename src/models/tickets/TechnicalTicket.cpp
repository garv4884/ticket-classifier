#include "models/tickets/TechnicalTicket.h"

#include <utility>
#include <vector>

#include "utils/TextUtils.h"

void TechnicalTicket::extractDetails() {
    static const std::regex errorPattern = text::labelledIdPattern("error code|error|err");
    // normalized keyword -> display name
    static const std::vector<std::pair<std::string, std::string>> platforms{
        {"android", "Android"}, {"iphone", "iPhone"}, {"ios", "iOS"},     {"ipad", "iPad"},
        {"windows", "Windows"}, {"macos", "macOS"},   {"mac", "Mac"},     {"linux", "Linux"},
        {"chrome", "Chrome"},   {"firefox", "Firefox"}, {"safari", "Safari"}};

    if (auto code = text::firstMatch(fullText(), errorPattern)) addDetail("error_code", *code);

    const std::string normalized = text::normalize(fullText());
    for (const auto& [keyword, displayName] : platforms) {
        if (text::containsPhrase(normalized, keyword)) {
            addDetail("platform", displayName);
            break;
        }
    }
}
