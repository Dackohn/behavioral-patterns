#pragma once

#include <memory>
#include "TicketCreationRequest.hpp"

namespace domain::behaviors::chain {

class ITicketHandler {
protected:
    std::shared_ptr<ITicketHandler> next;

public:
    virtual ~ITicketHandler() = default;

    void setNext(std::shared_ptr<ITicketHandler> nextHandler) {
        next = std::move(nextHandler);
    }

    void handle(TicketCreationRequest& request) {
        if (!request.valid) return;
        process(request);
        if (request.valid && next) {
            next->handle(request);
        }
    }

protected:
    virtual void process(TicketCreationRequest& request) = 0;
};

} // namespace domain::behaviors::chain
