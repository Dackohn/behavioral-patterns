#pragma once

#include <memory>
#include "../../../domain/interfaces/ILogger.hpp"
#include "ITicketObserver.hpp"

namespace domain::behaviors::observer {

class TicketLoggingObserver : public ITicketObserver {
private:
    std::shared_ptr<domain::ILogger> logger;

public:
    explicit TicketLoggingObserver(std::shared_ptr<domain::ILogger> log)
        : logger(std::move(log)) {}

    void onTicketStatusChanged(const std::string& ticketId,
                               TicketStatus oldStatus,
                               TicketStatus newStatus) override {
        if (logger) {
            logger->log("Ticket " + ticketId +
                        " status changed from " + std::to_string(static_cast<int>(oldStatus)) +
                        " to " + std::to_string(static_cast<int>(newStatus)));
        }
    }

    void onTicketCreated(const std::string& ticketId) override {
        if (logger) {
            logger->log("Ticket created: " + ticketId);
        }
    }
};

} // namespace domain::behaviors::observer
