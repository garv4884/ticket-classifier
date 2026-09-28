#include "repliers/BaseReplier.h"

#include "models/Ticket.h"
#include "utils/TextUtils.h"

std::string BaseReplier::generate(const Ticket& ticket) const {
    return "Hi there,\n\n" + opening(ticket) + "\n\n" + body(ticket) + "\n\n" +
           serviceLevel(ticket) + "\n\n" + closing(ticket);
}

std::string BaseReplier::opening(const Ticket& ticket) const {
    switch (ticket.sentiment().label) {
        case Sentiment::ANGRY:
            return "We are truly sorry for the frustration this has caused you. We understand how "
                   "serious this is, and we are treating it with the highest priority.";
        case Sentiment::NEGATIVE:
            return "We are sorry to hear you have run into this, and thank you for bringing it to "
                   "our attention.";
        case Sentiment::POSITIVE:
            return "Thank you for your kind words and for getting in touch - it really means a lot "
                   "to our team.";
        case Sentiment::NEUTRAL:
            break;
    }
    return "Thank you for contacting us.";
}

std::string BaseReplier::serviceLevel(const Ticket& ticket) const {
    std::string line;
    switch (ticket.priority()) {
        case Priority::P0:
            line = "Your ticket is marked P0 (critical). Our on-call team will respond within 1 hour.";
            break;
        case Priority::P1:
            line = "Your ticket is marked P1 (high). You can expect a response within 24 hours.";
            break;
        case Priority::P2:
            line = "Your ticket is marked P2 (normal). You can expect a response within 72 hours.";
            break;
    }
    if (ticket.escalationRequired()) {
        line += " Given the urgency, it has also been escalated to a senior support lead who will "
                "contact you personally.";
    }
    return line;
}

std::string BaseReplier::closing(const Ticket& ticket) const {
    return "Your reference number is " + ticket.id() +
           ". Just reply to this message if there is anything else we can help with.\n\n"
           "Best regards,\nCustomer Support Team";
}

std::string BaseReplier::matchSubTopic(const Ticket& ticket, const std::vector<SubTopic>& topics,
                                       const std::string& fallback) {
    const std::string normalized = text::normalize(ticket.fullText());
    for (const SubTopic& topic : topics) {
        for (const std::string& keyword : topic.keywords) {
            if (text::containsPhrase(normalized, keyword)) return topic.response;
        }
    }
    return fallback;
}
