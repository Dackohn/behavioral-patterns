#pragma once

#include <string>
#include "../../../domain/services/NotificationService.hpp"
#include "../../../domain/interfaces/ICustomerRepository.hpp"
#include "ITicketObserver.hpp"

namespace domain::behaviors::observer {

class TicketNotificationObserver : public ITicketObserver {
private:
    domain::NotificationService& notifier;
    domain::ICustomerRepository& customerRepo;

public:
    TicketNotificationObserver(domain::NotificationService& n,
                               domain::ICustomerRepository& repo)
        : notifier(n), customerRepo(repo) {}

    void onTicketStatusChanged(const std::string& ticketId,
                               TicketStatus /*oldStatus*/,
                               TicketStatus newStatus) override {
        // You could look up ticket + customer here and send an email/SMS.
        // For simplicity, we just log-notify:
        notifier.notify("support@example.com",
                        "Ticket " + ticketId + " changed to status " +
                        std::to_string(static_cast<int>(newStatus)));
    }

    void onTicketCreated(const std::string& ticketId) override {
        notifier.notify("support@example.com",
                        "New ticket created: " + ticketId);
    }
};

} // namespace domain::behaviors::observer
