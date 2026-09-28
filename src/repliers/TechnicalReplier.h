#pragma once

#include "repliers/BaseReplier.h"

class TechnicalReplier : public BaseReplier {
public:
    TechnicalReplier();

protected:
    std::string body(const Ticket& ticket) const override;

private:
    std::vector<SubTopic> topics_;
};
