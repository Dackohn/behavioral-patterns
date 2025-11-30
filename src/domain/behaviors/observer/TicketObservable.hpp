#pragma once

#include <memory>
#include <string>
#include <vector>
#include "../../models/Enums.hpp"
#include "ITicketObserver.hpp"

namespace domain::behaviors::observer {

class TicketObservable {
private:
    std::vector<std::shared_ptr<ITicketObserver>> observers;

public:
    void addObserver(std::shared_ptr<ITicketObserver> obs) {
        observers.push_back(std::move(obs));
    }

    void notifyTicketCreated(const std::string& ticketId) {
        for (auto& o : observers) {
            o->onTicketCreated(ticketId);
        }
    }

    void notifyTicketStatusChanged(const std::string& ticketId,
                                   TicketStatus oldStatus,
                                   TicketStatus newStatus) {
        for (auto& o : observers) {
            o->onTicketStatusChanged(ticketId, oldStatus, newStatus);
        }
    }
};

} // namespace domain::behaviors::observer
