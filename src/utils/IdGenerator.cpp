#include "utils/IdGenerator.h"

#include <iomanip>
#include <sstream>

IdGenerator::IdGenerator(int nextNumber) : nextNumber_(nextNumber) {}

std::string IdGenerator::next() {
    const int num = nextNumber_.fetch_add(1);
    std::ostringstream id;
    id << "TKT-" << std::setw(4) << std::setfill('0') << num;
    return id.str();
}
