#include "utils/IdGenerator.h"

#include <iomanip>
#include <sstream>

IdGenerator::IdGenerator(int nextNumber) : nextNumber_(nextNumber) {}

std::string IdGenerator::next() {
    std::ostringstream id;
    id << "TKT-" << std::setw(4) << std::setfill('0') << nextNumber_++;
    return id.str();
}
