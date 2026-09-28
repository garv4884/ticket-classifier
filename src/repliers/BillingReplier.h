#pragma once

#include "repliers/BaseReplier.h"

class BillingReplier : public BaseReplier {
public:
    BillingReplier();

protected:
    std::string body(const Ticket& ticket) const override;

private:
    std::vector<SubTopic> topics_;
};
