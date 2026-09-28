#include "repliers/ReturnsReplier.h"

#include "models/Ticket.h"

ReturnsReplier::ReturnsReplier()
    : topics_{
          {{"damaged", "broken", "defective", "cracked"},
           "We are sorry the item arrived in this condition. Please reply with a photo of the item "
           "and its packaging, and we will send a free replacement or a full refund - you do not "
           "need to wait for the damaged item to be picked up."},
          {{"wrong item", "wrong size", "wrong colour", "wrong color", "different item"},
           "Sorry for the mix-up. We will arrange a free pickup of the item you received and ship "
           "the correct one as soon as the pickup is scheduled."},
          {{"refund"},
           "Refunds are issued to your original payment method within 5-7 business days after the "
           "returned item passes inspection. If it has been longer than that, our team will trace "
           "the refund with the payment provider and update you."},
          {{"exchange", "replacement", "replace"},
           "Exchanges are free within 30 days of delivery. Choose Orders > Exchange Item and the new "
           "item will ship as soon as the pickup is completed."},
          {{"return", "returns", "pickup"},
           "Most items can be returned within 30 days of delivery. Start the return from Orders > "
           "Return Item and we will arrange a free pickup."},
      } {}

std::string ReturnsReplier::body(const Ticket& ticket) const {
    std::string reference = "Your request has been routed to our Returns & Refunds team.";
    if (auto order = ticket.detail("order_id")) reference += " We have located order " + *order + ".";
    return reference + "\n\n" +
           matchSubTopic(ticket, topics_,
                         "Our returns team will review your request and get back to you with the "
                         "next steps.");
}
