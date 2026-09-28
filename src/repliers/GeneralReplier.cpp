#include "repliers/GeneralReplier.h"

#include "models/Ticket.h"

GeneralReplier::GeneralReplier()
    : topics_{
          {{"suggestion", "feature", "feedback", "idea", "would be"},
           "Thank you for the suggestion! We have shared it with our product team, who review "
           "customer feedback every week when planning upcoming improvements."},
          {{"thanks", "thank", "great", "love", "loving", "awesome"},
           "We have passed your message on to the team - feedback like this makes their day."},
          {{"hours", "contact", "phone", "call", "speak"},
           "Our support team is available 24/7 via chat and email, and by phone Monday to Saturday "
           "from 9am to 9pm. You can find all contact options under Help > Contact Us."},
      } {}

std::string GeneralReplier::body(const Ticket& ticket) const {
    return "Your message has been routed to our General Support team.\n\n" +
           matchSubTopic(ticket, topics_,
                         "A member of our support team will review your message and reply "
                         "shortly.");
}
