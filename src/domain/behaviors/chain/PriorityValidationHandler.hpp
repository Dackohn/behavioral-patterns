#pragma once

#include "ITicketHandler.hpp"

namespace domain::behaviors::chain {

class PriorityValidationHandler : public ITicketHandler {
protected:
    void process(TicketCreationRequest& request) override {
        // Example rule: CRITICAL must be longer description
        if (request.priority == domain::Priority::CRITICAL &&
            request.description.size() < 20) {
            request.valid = false;
            request.errorMessage =
                "CRITICAL tickets must have detailed descriptions (>= 20 chars).";
        }
    }
};

} // namespace domain::behaviors::chain
