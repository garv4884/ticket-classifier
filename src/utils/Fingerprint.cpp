#include "utils/Fingerprint.h"

#include <cstdint>
#include <iomanip>
#include <sstream>

#include "utils/TextUtils.h"

namespace fingerprint {

std::string of(const std::string& subject, const std::string& description) {
    const std::string canonical = text::normalize(subject) + "|" + text::normalize(description);

    std::uint64_t hash = 14695981039346656037ULL;  // FNV-1a offset basis
    for (unsigned char c : canonical) {
        hash ^= c;
        hash *= 1099511628211ULL;  // FNV-1a prime
    }

    std::ostringstream out;
    out << std::hex << std::setw(16) << std::setfill('0') << hash;
    return out.str();
}

}  // namespace fingerprint
