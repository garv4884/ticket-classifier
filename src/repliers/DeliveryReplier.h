#pragma once

#include "repliers/BaseReplier.h"

class DeliveryReplier : public BaseReplier {
public:
    DeliveryReplier();

protected:
    std::string body(const Ticket& ticket) const override;

private:
    std::vector<SubTopic> topics_;
};
