#pragma once

#include <string>
#include <utility>
#include <vector>

#include "services/classification/IClassifier.h"

// Weighted keyword scoring. For every category, each keyword/phrase found in
// the subject scores weight * 2 and in the description weight * 1 (the subject
// is usually the clearest signal). The highest total wins; below kMinScore the
// ticket falls back to GENERAL. Ties go to the category listed first.
class KeywordClassifier : public IClassifier {
public:
    KeywordClassifier();

    ClassificationResult classify(const std::string& subject,
                                  const std::string& description) const override;

private:
    struct Rule {
        Category category;
        std::vector<std::pair<std::string, int>> keywords;  // normalized phrase -> weight
    };

    static constexpr int kSubjectWeight = 2;
    static constexpr int kDescriptionWeight = 1;
    static constexpr int kMinScore = 2;

    std::vector<Rule> rules_;
};
