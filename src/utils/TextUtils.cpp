#include "utils/TextUtils.h"

#include <algorithm>
#include <cctype>
#include <ctime>

namespace text {

namespace {

bool isWordChar(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) != 0;
}

}  // namespace

std::string trim(const std::string& value) {
    const auto begin = value.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos) return "";
    const auto end = value.find_last_not_of(" \t\r\n");
    return value.substr(begin, end - begin + 1);
}

std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}

std::string toUpper(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return value;
}

std::string normalize(const std::string& value) {
    std::string result;
    result.reserve(value.size());
    bool pendingSpace = false;
    for (char c : value) {
        if (c == '\'') continue;
        if (!isWordChar(c)) {
            pendingSpace = true;
            continue;
        }
        if (pendingSpace && !result.empty()) result += ' ';
        pendingSpace = false;
        result += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return result;
}

std::vector<std::string> tokenize(const std::string& value) {
    std::vector<std::string> tokens;
    std::string current;
    for (char c : value) {
        if (c == '\'') continue;
        if (isWordChar(c)) {
            current += c;
        } else if (!current.empty()) {
            tokens.push_back(current);
            current.clear();
        }
    }
    if (!current.empty()) tokens.push_back(current);
    return tokens;
}

bool containsPhrase(const std::string& normalizedText, const std::string& phrase) {
    const std::string haystack = " " + normalizedText + " ";
    return haystack.find(" " + phrase + " ") != std::string::npos;
}

std::regex labelledIdPattern(const std::string& labelAlternatives) {
    // label, optional "id/no/number", optional "is", optional '#' or ':', then the id.
    // The id must have at least one digit and consist of alphanumeric chars and hyphens.
    // Avoiding ambiguous overlapping nested quantifiers (e.g. ([A-Za-z]*-?\d...)) prevents catastrophic backtracking (ReDoS).
    return std::regex("\\b(?:" + labelAlternatives +
                          ")\\s*(?:id|no\\.?|number)?\\s*(?:is\\s+)?[#:]?\\s*([A-Za-z0-9]+(?:-[A-Za-z0-9]+)*)",
                      std::regex::icase);
}

std::optional<std::string> firstMatch(const std::string& value, const std::regex& pattern,
                                      std::size_t group) {
    std::smatch match;
    if (std::regex_search(value, match, pattern) && match.size() > group && match[group].matched) {
        return trim(match[group].str());
    }
    return std::nullopt;
}

std::vector<std::string> split(const std::string& value, char delimiter) {
    std::vector<std::string> parts;
    std::string current;
    for (char c : value) {
        if (c == delimiter) {
            parts.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }
    parts.push_back(current);
    return parts;
}

std::string join(const std::vector<std::string>& parts, const std::string& separator) {
    std::string result;
    for (std::size_t i = 0; i < parts.size(); ++i) {
        if (i > 0) result += separator;
        result += parts[i];
    }
    return result;
}

std::string escapeField(const std::string& value) {
    std::string result;
    for (char c : value) {
        switch (c) {
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\t': result += "\\t"; break;
            case '\r': break;
            default:   result += c;
        }
    }
    return result;
}

std::string unescapeField(const std::string& value) {
    std::string result;
    for (std::size_t i = 0; i < value.size(); ++i) {
        if (value[i] == '\\' && i + 1 < value.size()) {
            const char next = value[++i];
            result += next == 'n' ? '\n' : next == 't' ? '\t' : next;
        } else {
            result += value[i];
        }
    }
    return result;
}

std::string currentTimestamp() {
    const std::time_t now = std::time(nullptr);
    std::tm local{};
#if defined(_WIN32) || defined(_MSC_VER)
    localtime_s(&local, &now);
#else
    localtime_r(&now, &local);
#endif
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local);
    return buffer;
}

}  // namespace text
