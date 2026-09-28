#pragma once

#include <string>

#include "models/AnalysisResults.h"

// Strategy: decides which category (destination) a ticket belongs to.
class IClassifier {
public:
    virtual ~IClassifier() = default;

    virtual ClassificationResult classify(const std::string& subject,
                                          const std::string& description) const = 0;
};
