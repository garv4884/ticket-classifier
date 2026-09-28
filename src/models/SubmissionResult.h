#pragma once

#include <map>
#include <string>
#include <vector>

#include "models/Enums.h"

enum class SubmissionStatus { CREATED, DUPLICATE, REJECTED };

inline std::string toString(SubmissionStatus status) {
    switch (status) {
        case SubmissionStatus::CREATED:   return "CREATED";
        case SubmissionStatus::DUPLICATE: return "DUPLICATE";
        case SubmissionStatus::REJECTED:  return "REJECTED";
    }
    return "UNKNOWN";
}

// What TicketService hands back to a controller for display.
struct SubmissionResult {
    SubmissionStatus status = SubmissionStatus::REJECTED;
    std::string message;  // rejection reason or duplicate note
    std::string ticketId;
    Priority priority = Priority::P2;
    Category category = Category::GENERAL;
    Sentiment sentiment = Sentiment::NEUTRAL;
    int sentimentScore = 0;
    std::vector<std::string> matchedKeywords;
    std::map<std::string, std::string> details;
    std::vector<std::string> lifecycle;
    std::string finalState;
    std::string reply;
};
