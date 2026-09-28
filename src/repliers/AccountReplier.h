#pragma once

#include "repliers/BaseReplier.h"

class AccountReplier : public BaseReplier {
public:
    AccountReplier();

protected:
    std::string body(const Ticket& ticket) const override;

private:
    std::vector<SubTopic> topics_;
};
