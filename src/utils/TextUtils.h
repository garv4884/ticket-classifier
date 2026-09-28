#pragma once

#include <cstddef>
#include <optional>
#include <regex>
#include <string>
#include <vector>

namespace text {

std::string trim(const std::string& value);
std::string toLower(std::string value);
std::string toUpper(std::string value);

// Lowercases, drops apostrophes ("don't" -> "dont") and turns every other
// non-alphanumeric run into a single space. Used for keyword matching and
// for building idempotency keys.
std::string normalize(const std::string& value);

// Splits on non-alphanumeric characters but keeps the original case, since
// the sentiment analyzer needs it to spot SHOUTING.
std::vector<std::string> tokenize(const std::string& value);

// Whole-word match of a normalized phrase inside normalized text, so "bill"
// does not match "billion" and "log in" matches only as a phrase.
bool containsPhrase(const std::string& normalizedText, const std::string& phrase);

// Pattern for an identifier written after a label, e.g. "order #48213",
// "Order ID: ORD-5521", "tracking number TRK-9". The id must contain a digit.
std::regex labelledIdPattern(const std::string& labelAlternatives);

std::optional<std::string> firstMatch(const std::string& value, const std::regex& pattern,
                                      std::size_t group = 1);

std::vector<std::string> split(const std::string& value, char delimiter);
std::string join(const std::vector<std::string>& parts, const std::string& separator);

// Escape so a multi-line value fits in one tab-separated field, and back.
std::string escapeField(const std::string& value);
std::string unescapeField(const std::string& value);

std::string currentTimestamp();

}  // namespace text
