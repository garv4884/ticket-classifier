#pragma once

#include <string>
#include <vector>

#include "models/Enums.h"

struct ClassificationResult {
    Category category = Category::GENERAL;
    int score = 0;
    std::vector<std::string> matchedKeywords;
};

struct SentimentResult {
    Sentiment label = Sentiment::NEUTRAL;
    int score = 0;
};
