#pragma once

#include <optional>
#include <string>
#include <vector>

// P0 is the highest priority.
enum class Priority { P0, P1, P2 };

// Every category maps to its own destination file in the repository.
enum class Category { BILLING, TECHNICAL, DELIVERY, ACCOUNT, RETURNS, GENERAL };

enum class Sentiment { POSITIVE, NEUTRAL, NEGATIVE, ANGRY };

std::string toString(Priority priority);
std::string toString(Category category);
std::string toString(Sentiment sentiment);

std::optional<Priority> parsePriority(const std::string& text);
std::optional<Category> parseCategory(const std::string& text);
std::optional<Sentiment> parseSentiment(const std::string& text);

const std::vector<Category>& allCategories();
