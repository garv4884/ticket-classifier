#pragma once

#include "repliers/BaseReplier.h"

class ReturnsReplier : public BaseReplier {
public:
    ReturnsReplier();

protected:
    std::string body(const Ticket& ticket) const override;

private:
    std::vector<SubTopic> topics_;
};
