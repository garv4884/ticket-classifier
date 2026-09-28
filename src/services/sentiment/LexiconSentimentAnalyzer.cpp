#include "services/sentiment/LexiconSentimentAnalyzer.h"

#include <algorithm>
#include <cctype>

#include "utils/TextUtils.h"

LexiconSentimentAnalyzer::LexiconSentimentAnalyzer()
    : lexicon_{
          // positive
          {"thanks", 1}, {"thank", 1}, {"good", 1}, {"nice", 1}, {"quick", 1}, {"smooth", 1},
          {"great", 2}, {"love", 2}, {"loving", 2}, {"happy", 2}, {"glad", 2}, {"pleased", 2},
          {"appreciate", 2}, {"appreciated", 2}, {"helpful", 2}, {"satisfied", 2},
          {"excellent", 3}, {"awesome", 3}, {"amazing", 3}, {"perfect", 3}, {"wonderful", 3},
          {"fantastic", 3},
          // negative
          {"unfortunately", -1}, {"wrong", -1}, {"stuck", -1}, {"delayed", -1}, {"confused", -1},
          {"bad", -2}, {"poor", -2}, {"upset", -2}, {"annoyed", -2}, {"annoying", -2},
          {"frustrated", -2}, {"frustrating", -2}, {"disappointed", -2}, {"disappointing", -2},
          {"waste", -2}, {"scared", -2}, {"worried", -2},
          {"terrible", -3}, {"horrible", -3}, {"awful", -3}, {"hate", -3}, {"angry", -3},
          {"unacceptable", -3}, {"ridiculous", -3}, {"useless", -3}, {"worse", -3},
          {"disaster", -3}, {"fraud", -3}, {"cheated", -3},
          {"worst", -4}, {"furious", -4}, {"pathetic", -4}, {"scam", -4}, {"disgusting", -4},
          {"incompetent", -4},
      },
      negators_{"not", "no", "never", "dont", "doesnt", "didnt", "isnt", "wasnt", "arent",
                "cant", "cannot", "couldnt", "wont", "wouldnt", "havent", "hasnt", "hadnt",
                "neither", "nor"},
      intensifiers_{"very", "really", "so", "extremely", "totally", "absolutely", "completely",
                    "super", "incredibly", "highly"} {}

SentimentResult LexiconSentimentAnalyzer::analyze(const std::string& input) const {
    int score = 0;
    int negationWindow = 0;     // words a preceding negator still applies to
    int intensifierWindow = 0;  // words a preceding intensifier still applies to

    for (const std::string& token : text::tokenize(input)) {
        const std::string word = text::toLower(token);

        if (negators_.count(word)) {
            negationWindow = kNegationReach;
            continue;
        }
        if (intensifiers_.count(word)) {
            intensifierWindow = kIntensifierReach;
            continue;
        }

        const auto entry = lexicon_.find(word);
        if (entry != lexicon_.end()) {
            int value = entry->second;
            if (negationWindow > 0) {
                value = -value;
                negationWindow = 0;
            }
            if (intensifierWindow > 0) {
                value *= 2;
                intensifierWindow = 0;
            }
            if (value < 0 && isShouting(token)) value -= 1;
            score += value;
        }

        if (negationWindow > 0) --negationWindow;
        if (intensifierWindow > 0) --intensifierWindow;
    }

    if (score < 0) {
        const int exclamations = static_cast<int>(std::count(input.begin(), input.end(), '!'));
        score -= std::min(kMaxExclamationPenalty, exclamations / 2);
    }

    return {labelFor(score), score};
}

bool LexiconSentimentAnalyzer::isShouting(const std::string& token) {
    if (token.size() < 3) return false;
    return std::all_of(token.begin(), token.end(), [](unsigned char c) {
        return !std::isalpha(c) || std::isupper(c);
    });
}

Sentiment LexiconSentimentAnalyzer::labelFor(int score) {
    if (score <= kAngryThreshold) return Sentiment::ANGRY;
    if (score < 0) return Sentiment::NEGATIVE;
    if (score == 0) return Sentiment::NEUTRAL;
    return Sentiment::POSITIVE;
}
