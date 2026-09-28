#include "repliers/DeliveryReplier.h"

#include "models/Ticket.h"

DeliveryReplier::DeliveryReplier()
    : topics_{
          {{"not delivered", "never arrived", "not received", "not arrived", "missing", "lost"},
           "We have opened a trace with the courier. If the package cannot be located within 48 "
           "hours, we will either reship it at no cost or issue a full refund - whichever you "
           "prefer."},
          {{"late", "delayed", "delay", "still waiting", "taking too long"},
           "We apologise for the delay. We have asked the courier for an updated delivery date and "
           "will share it with you as soon as we hear back, usually within 24 hours."},
          {{"address", "reschedule", "change delivery"},
           "We can update the delivery address or reschedule as long as the package has not yet "
           "been handed to the local courier. Please reply with the new address or preferred date."},
          {{"tracking", "track"},
           "You can follow live updates from Orders > Track Package. It can take up to 24 hours "
           "after dispatch for the first tracking scan to appear."},
      } {}

std::string DeliveryReplier::body(const Ticket& ticket) const {
    std::string reference = "Your request has been routed to our Delivery team.";
    if (auto order = ticket.detail("order_id")) reference += " We are looking into order " + *order + ".";
    if (auto tracking = ticket.detail("tracking_id")) {
        reference += " Tracking number " + *tracking + " has been flagged with the courier.";
    }
    return reference + "\n\n" +
           matchSubTopic(ticket, topics_,
                         "Our delivery team will check the shipment status and update you shortly.");
}
