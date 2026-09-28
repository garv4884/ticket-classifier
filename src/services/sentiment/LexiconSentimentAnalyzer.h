#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "services/sentiment/ISentimentAnalyzer.h"

// Lexicon-based scoring:
//  - each known word adds its polarity (e.g. "great" +2, "worst" -4)
//  - a negator ("not", "never", "dont", ...) flips the next sentiment word
//    within kNegationReach words ("not happy" -> negative)
//  - an intensifier ("very", "absolutely", ...) doubles the next sentiment word
//  - an ALL-CAPS negative word and repeated "!!" make a negative score stronger
// Labels: score <= kAngryThreshold ANGRY, < 0 NEGATIVE, 0 NEUTRAL, > 0 POSITIVE.
class LexiconSentimentAnalyzer : public ISentimentAnalyzer {
public:
    LexiconSentimentAnalyzer();

    SentimentResult analyze(const std::string& text) const override;

private:
    static constexpr int kNegationReach = 3;
    static constexpr int kIntensifierReach = 2;
    static constexpr int kMaxExclamationPenalty = 3;
    static constexpr int kAngryThreshold = -5;

    static bool isShouting(const std::string& token);
    static Sentiment labelFor(int score);

    std::unordered_map<std::string, int> lexicon_;
    std::unordered_set<std::string> negators_;
    std::unordered_set<std::string> intensifiers_;
};
