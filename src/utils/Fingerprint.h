#pragma once

#include <string>

namespace fingerprint {

// Idempotency key for a ticket: a stable 64-bit FNV-1a hash of the normalized
// subject and description, so resubmitting the same issue with different
// casing, punctuation or spacing maps to the same key. Priority is left out on
// purpose: the same issue sent again as P0 is still the same ticket.
std::string of(const std::string& subject, const std::string& description);

}  // namespace fingerprint
