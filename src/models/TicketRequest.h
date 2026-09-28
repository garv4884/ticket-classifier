#pragma once

#include <string>

// Raw input exactly as a controller received it. Priority stays a string so
// validation happens in one place (TicketService) instead of every controller.
struct TicketRequest {
    std::string subject;
    std::string description;
    std::string priority;
};
