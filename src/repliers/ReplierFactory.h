#pragma once

#include <map>
#include <memory>

#include "models/Enums.h"
#include "repliers/IReplier.h"

// Factory: returns the replier for a category. Repliers are stateless, so one
// instance per category is created up front and reused.
class ReplierFactory {
public:
    ReplierFactory();

    const IReplier& forCategory(Category category) const;

private:
    std::map<Category, std::unique_ptr<IReplier>> repliers_;
};
