#pragma once

#include <string>

// Produces TKT-0001, TKT-0002, ... starting from the given number, so a new
// run can continue numbering after the tickets already stored.
class IdGenerator {
public:
    explicit IdGenerator(int nextNumber = 1);

    std::string next();

private:
    int nextNumber_;
};
