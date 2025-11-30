#pragma once

#include <string>
#include "../../models/Enums.hpp"

namespace domain::behaviors::observer {

class ITicketObserver {
public:
    virtual ~ITicketObserver() = default;

    virtual void onTicketStatusChanged(const std::string& ticketId,
                                       TicketStatus oldStatus,
                                       TicketStatus newStatus) = 0;

    virtual void onTicketCreated(const std::string& ticketId) = 0;
};

} // namespace domain::behaviors::observer
