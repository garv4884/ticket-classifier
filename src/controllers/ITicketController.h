#pragma once

// An entry point through which tickets reach TicketService.
class ITicketController {
public:
    virtual ~ITicketController() = default;

    virtual void run() = 0;
};
