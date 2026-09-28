#pragma once

#include <string>

#include "models/AnalysisResults.h"

// Strategy: estimates how the customer feels, which drives the tone of the reply.
class ISentimentAnalyzer {
public:
    virtual ~ISentimentAnalyzer() = default;

    virtual SentimentResult analyze(const std::string& text) const = 0;
};
