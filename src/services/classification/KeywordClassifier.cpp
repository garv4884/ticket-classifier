#include "services/classification/KeywordClassifier.h"

#include "utils/TextUtils.h"

KeywordClassifier::KeywordClassifier()
    : rules_{
          {Category::ACCOUNT,
           {{"login", 3}, {"log in", 3}, {"sign in", 3}, {"password", 3}, {"locked out", 3},
            {"locked", 2}, {"2fa", 3}, {"two factor", 3}, {"otp", 2}, {"verification code", 2},
            {"username", 2}, {"hacked", 3}, {"unauthorized", 3}, {"unauthorised", 3},
            {"account", 1}, {"reset", 1}}},
          {Category::BILLING,
           {{"invoice", 3}, {"billing", 3}, {"billed", 3}, {"bill", 2}, {"charged", 3},
            {"charge", 2}, {"charged twice", 4}, {"double charged", 4}, {"overcharged", 3},
            {"payment", 3}, {"deducted", 3}, {"subscription", 2}, {"credit card", 3},
            {"card", 2}, {"receipt", 2}, {"tax", 2}, {"price", 1}, {"refund", 1}}},
          {Category::RETURNS,
           {{"return", 3}, {"returns", 3}, {"returned", 3}, {"returning", 3}, {"refund", 2},
            {"replacement", 3}, {"replace", 2}, {"exchange", 3}, {"damaged", 3},
            {"defective", 3}, {"broken", 2}, {"wrong item", 4}, {"wrong size", 3}}},
          {Category::DELIVERY,
           {{"delivery", 3}, {"delivered", 3}, {"deliver", 2}, {"shipping", 3}, {"shipped", 3},
            {"shipment", 3}, {"tracking", 3}, {"courier", 3}, {"parcel", 3}, {"package", 2},
            {"out for delivery", 3}, {"dispatched", 2}, {"arrived", 2}, {"arrive", 2},
            {"delayed", 2}, {"late", 1}}},
          {Category::TECHNICAL,
           {{"crash", 3}, {"crashes", 3}, {"crashing", 3}, {"crashed", 3}, {"freeze", 3},
            {"freezes", 3}, {"frozen", 3}, {"error", 3}, {"bug", 3}, {"glitch", 3},
            {"install", 3}, {"installation", 3}, {"404", 3}, {"not working", 2}, {"slow", 2},
            {"loading", 2}, {"update", 2}, {"sync", 2}, {"app", 1}, {"website", 1},
            {"load", 1}}},
      } {}

ClassificationResult KeywordClassifier::classify(const std::string& subject,
                                                 const std::string& description) const {
    const std::string normalizedSubject = text::normalize(subject);
    const std::string normalizedDescription = text::normalize(description);

    ClassificationResult best;
    for (const Rule& rule : rules_) {
        int score = 0;
        std::vector<std::string> matched;
        for (const auto& [keyword, weight] : rule.keywords) {
            int hit = 0;
            if (text::containsPhrase(normalizedSubject, keyword)) hit += kSubjectWeight * weight;
            if (text::containsPhrase(normalizedDescription, keyword)) hit += kDescriptionWeight * weight;
            if (hit > 0) {
                score += hit;
                matched.push_back(keyword);
            }
        }
        if (score > best.score) best = {rule.category, score, matched};
    }

    if (best.score < kMinScore) return {Category::GENERAL, 0, {}};
    return best;
}
