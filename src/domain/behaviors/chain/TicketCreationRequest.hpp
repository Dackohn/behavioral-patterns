#pragma once

#include <string>
#include "../../models/Enums.hpp"

namespace domain::behaviors::chain {

struct TicketCreationRequest {
    std::string customerId;
    std::string description;
    domain::Priority priority;
    domain::TicketCategory category;

    bool valid = true;
    std::string errorMessage;
};

} // namespace domain::behaviors::chain
