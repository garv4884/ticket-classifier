#include "models/Enums.h"

#include "utils/TextUtils.h"

std::string toString(Priority priority) {
    switch (priority) {
        case Priority::P0: return "P0";
        case Priority::P1: return "P1";
        case Priority::P2: return "P2";
    }
    return "UNKNOWN";
}

std::string toString(Category category) {
    switch (category) {
        case Category::BILLING:   return "BILLING";
        case Category::TECHNICAL: return "TECHNICAL";
        case Category::DELIVERY:  return "DELIVERY";
        case Category::ACCOUNT:   return "ACCOUNT";
        case Category::RETURNS:   return "RETURNS";
        case Category::GENERAL:   return "GENERAL";
    }
    return "UNKNOWN";
}

std::string toString(Sentiment sentiment) {
    switch (sentiment) {
        case Sentiment::POSITIVE: return "POSITIVE";
        case Sentiment::NEUTRAL:  return "NEUTRAL";
        case Sentiment::NEGATIVE: return "NEGATIVE";
        case Sentiment::ANGRY:    return "ANGRY";
    }
    return "UNKNOWN";
}

std::optional<Priority> parsePriority(const std::string& text) {
    const std::string value = text::toUpper(text::trim(text));
    if (value == "P0") return Priority::P0;
    if (value == "P1") return Priority::P1;
    if (value == "P2") return Priority::P2;
    return std::nullopt;
}

std::optional<Category> parseCategory(const std::string& text) {
    const std::string value = text::toUpper(text::trim(text));
    for (Category category : allCategories()) {
        if (toString(category) == value) return category;
    }
    return std::nullopt;
}

std::optional<Sentiment> parseSentiment(const std::string& text) {
    const std::string value = text::toUpper(text::trim(text));
    for (Sentiment sentiment : {Sentiment::POSITIVE, Sentiment::NEUTRAL, Sentiment::NEGATIVE, Sentiment::ANGRY}) {
        if (toString(sentiment) == value) return sentiment;
    }
    return std::nullopt;
}

const std::vector<Category>& allCategories() {
    static const std::vector<Category> categories{
        Category::BILLING, Category::TECHNICAL, Category::DELIVERY,
        Category::ACCOUNT, Category::RETURNS,   Category::GENERAL};
    return categories;
}
