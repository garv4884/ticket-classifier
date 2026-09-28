#pragma once

#include <string>
#include <vector>

#include "repliers/IReplier.h"

// Template Method: every reply has the same skeleton - greeting, an empathy
// line chosen by sentiment, a category-specific body, an SLA line chosen by
// priority, and a sign-off. Subclasses only supply body().
class BaseReplier : public IReplier {
public:
    std::string generate(const Ticket& ticket) const final;

protected:
    // A narrower topic inside a category, e.g. "duplicate charge" within billing.
    struct SubTopic {
        std::vector<std::string> keywords;  // normalized words/phrases
        std::string response;
    };

    virtual std::string opening(const Ticket& ticket) const;
    virtual std::string body(const Ticket& ticket) const = 0;
    virtual std::string serviceLevel(const Ticket& ticket) const;
    virtual std::string closing(const Ticket& ticket) const;

    // The first sub-topic with a keyword present in the ticket wins, so list
    // the most specific topics first.
    static std::string matchSubTopic(const Ticket& ticket, const std::vector<SubTopic>& topics,
                                     const std::string& fallback);
};
