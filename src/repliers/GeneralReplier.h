#pragma once

#include "repliers/BaseReplier.h"

class GeneralReplier : public BaseReplier {
public:
    GeneralReplier();

protected:
    std::string body(const Ticket& ticket) const override;

private:
    std::vector<SubTopic> topics_;
};
