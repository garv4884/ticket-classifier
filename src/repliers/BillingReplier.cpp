#include "repliers/BillingReplier.h"

#include "models/Ticket.h"

BillingReplier::BillingReplier()
    : topics_{
          {{"charged twice", "double charged", "deducted twice", "deducted two times",
            "duplicate charge", "duplicate payment"},
           "Duplicate charges are often temporary authorisation holds that your bank releases "
           "within 3-5 business days. If the second charge has already settled, our billing team "
           "will reverse it and email you a confirmation once the refund is issued."},
          {{"overcharged", "extra charge", "wrong amount", "hidden charge"},
           "We will compare the amount you were charged with your plan and any applicable taxes, "
           "and refund any difference to your original payment method."},
          {{"payment failed", "failed payment", "declined", "transaction failed"},
           "Failed payments are usually caused by a bank-side decline or outdated card details. If "
           "money left your account for a failed payment, it is automatically returned within 5-7 "
           "business days. You can safely retry from Settings > Billing - a retried payment is "
           "never charged twice."},
          {{"cancel", "subscription", "renewal", "auto renew"},
           "You can manage or cancel your subscription at any time from Settings > Subscription. "
           "Cancelling stops the next renewal, and you keep access until the end of the current "
           "billing period."},
          {{"invoice", "receipt", "gst", "vat", "tax"},
           "All invoices can be downloaded from Settings > Billing > Invoices. If you need a "
           "corrected invoice (for example with a GST/VAT number or company name), reply with the "
           "details and we will reissue it."},
      } {}

std::string BillingReplier::body(const Ticket& ticket) const {
    std::string reference = "Your request has been routed to our Billing team.";
    if (auto invoice = ticket.detail("invoice_id")) reference += " We have pulled up " + *invoice + ".";
    if (auto amount = ticket.detail("amount")) {
        reference += " The amount in question (" + *amount + ") has been flagged for review.";
    }
    return reference + "\n\n" +
           matchSubTopic(ticket, topics_,
                         "A billing specialist will review your account and send you a detailed "
                         "breakdown of the charges.");
}
