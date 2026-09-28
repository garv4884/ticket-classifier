#include "controllers/ResultPrinter.h"

#include <sstream>
#include <string>
#include <vector>

#include "utils/TextUtils.h"

void printResult(std::ostream& out, const SubmissionResult& result) {
    out << "[" << toString(result.status) << "] ";
    if (result.status == SubmissionStatus::REJECTED) {
        out << result.message << "\n";
        return;
    }
    out << result.ticketId << "\n";
    if (!result.message.empty()) out << "  Note       : " << result.message << "\n";

    out << "  Routed to  : " << toString(result.category);
    if (!result.matchedKeywords.empty()) {
        out << "   (matched: " << text::join(result.matchedKeywords, ", ") << ")";
    }
    out << "\n  Sentiment  : " << toString(result.sentiment) << " (score " << result.sentimentScore << ")\n";

    if (!result.details.empty()) {
        std::vector<std::string> details;
        for (const auto& [key, value] : result.details) details.push_back(key + "=" + value);
        out << "  Details    : " << text::join(details, ", ") << "\n";
    }
    if (!result.lifecycle.empty()) {
        out << "  Lifecycle  : " << text::join(result.lifecycle, " -> ") << "\n";
    } else {
        out << "  State      : " << result.finalState << "\n";
    }

    out << "  Reply      :\n";
    std::istringstream lines(result.reply);
    std::string line;
    while (std::getline(lines, line)) out << "    | " << line << "\n";
}
