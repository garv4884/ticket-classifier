#include "repliers/AccountReplier.h"

#include "models/Ticket.h"

AccountReplier::AccountReplier()
    : topics_{
          {{"hacked", "unauthorized", "unauthorised", "suspicious", "compromised"},
           "For your security we have temporarily locked the account and signed out all active "
           "sessions. Please reset your password with the link we are sending to your registered "
           "email and turn on two-factor authentication. Any orders or changes you did not make "
           "will be reviewed and reversed."},
          {{"locked out", "locked", "blocked", "suspended", "disabled"},
           "Accounts are locked automatically after several failed sign-in attempts. The lock "
           "lifts after 30 minutes, or you can unlock it right away with the 'Forgot password' "
           "link."},
          {{"password", "forgot", "reset"},
           "Please use the 'Forgot password' link on the sign-in page. The reset email can take a "
           "few minutes to arrive, so please check your spam folder as well. The link stays valid "
           "for 60 minutes."},
          {{"2fa", "two factor", "otp", "verification code"},
           "If your verification code is not arriving, check that your phone number is correct "
           "under Settings > Security and wait 60 seconds before requesting a new code. You can "
           "also sign in with one of your backup codes."},
          {{"delete", "deactivate", "close my account"},
           "You can delete your account from Settings > Privacy > Delete Account. Deletion is "
           "permanent after a 14-day grace period, during which you can still change your mind."},
          {{"change email", "update email", "username"},
           "You can update your email or username from Settings > Profile. We will send a "
           "confirmation link to the new address before the change takes effect."},
      } {}

std::string AccountReplier::body(const Ticket& ticket) const {
    std::string reference = "Your request has been routed to our Account & Security team.";
    if (auto email = ticket.detail("email")) reference += " We will send all updates to " + *email + ".";
    return reference + "\n\n" +
           matchSubTopic(ticket, topics_,
                         "An account specialist will verify your identity and help you with the "
                         "request shortly.");
}
