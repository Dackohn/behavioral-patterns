#ifndef SUPPORT_FACADE_HPP
#define SUPPORT_FACADE_HPP

#include <memory>
#include <string>
#include <utility>

#include "../models/Enums.hpp"
#include "CustomerService.hpp"
#include "TicketService.hpp"
#include "NotificationService.hpp"

namespace domain {

// Facade over CustomerService, TicketService and NotificationService
class SupportFacade {
private:
    std::shared_ptr<CustomerService> customerService;
    std::shared_ptr<TicketService>   ticketService;
    NotificationService&             notifier;

public:
    SupportFacade(std::shared_ptr<CustomerService> cs,
                  std::shared_ptr<TicketService> ts,
                  NotificationService& n)
        : customerService(std::move(cs))
        , ticketService(std::move(ts))
        , notifier(n) {}

    std::pair<std::string, std::string> registerCustomerAndOpenTicket(
        const std::string& name,
        const std::string& email,
        const std::string& phone,
        const std::string& issueDescription,
        Priority priority,
        TicketCategory category
    ) {
        const std::string customerId = customerService->registerCustomer(
            name,
            email,
            phone
        );

        if (customerId.empty()) {
            return {"", ""};
        }

        const std::string ticketId = ticketService->createTicket(
            customerId,
            issueDescription,
            priority,
            category
        );

        if (ticketId.empty()) {
            return {customerId, ""};
        }

        notifier.notify(
            email,
            "Hello " + name +
            ", your support ticket " + ticketId +
            " has been created. Our team will contact you soon."
        );

        return {customerId, ticketId};
    }
};

} // namespace domain

#endif
