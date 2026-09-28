#include "repliers/TechnicalReplier.h"

#include "models/Ticket.h"

TechnicalReplier::TechnicalReplier()
    : topics_{
          {{"crash", "crashes", "crashing", "crashed", "freeze", "freezes", "frozen", "hangs"},
           "Please make sure you are on the latest version of the app and restart your device. If "
           "it still crashes, clearing the app cache or reinstalling resolves most cases. If the "
           "problem continues, reply with the exact steps that trigger it and our engineers will "
           "investigate."},
          {{"slow", "loading", "load", "lag", "laggy", "timeout"},
           "Slow performance is most often caused by network conditions or an old cached version. "
           "Please try a hard refresh (Ctrl+F5) or clear your browser/app cache, and check whether "
           "it also happens on another network. We are checking our servers for any slowdown on our "
           "side as well."},
          {{"install", "installation", "update", "upgrade", "download"},
           "Please check that your device meets the minimum requirements and has at least 500 MB "
           "of free space, then retry the installation from the official app store. If it still "
           "fails, a screenshot of the message will help us pinpoint the cause."},
          {{"sync", "syncing", "not saving"},
           "Please sign out and sign back in to force a fresh sync. Your data is stored safely on "
           "our servers, so nothing is lost while we look into it."},
          {{"error", "bug", "glitch", "404", "not working"},
           "Our engineers will look into the error you reported. A screenshot of the message and "
           "the time it happened would help us resolve it faster."},
      } {}

std::string TechnicalReplier::body(const Ticket& ticket) const {
    std::string reference = "Your request has been routed to our Technical Support team.";
    if (auto code = ticket.detail("error_code")) {
        reference += " We have noted error code " + *code +
                     ", which helps our engineers trace the exact failure.";
    }
    if (auto platform = ticket.detail("platform")) {
        reference += " We will reproduce the issue on " + *platform + ".";
    }
    return reference + "\n\n" +
           matchSubTopic(ticket, topics_,
                         "An engineer from our technical team will investigate and get back to "
                         "you with next steps.");
}
