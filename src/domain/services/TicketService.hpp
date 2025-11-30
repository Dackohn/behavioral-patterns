#ifndef TICKET_SERVICE_HPP
#define TICKET_SERVICE_HPP

#include <memory>
#include <string>
#include <vector>

#include "../models/Ticket.hpp"
#include "../models/Enums.hpp"
#include "../interfaces/ILogger.hpp"
#include "../interfaces/ITicketRepository.hpp"
#include "../interfaces/ICustomerRepository.hpp"
#include "../factory/TicketFactory.hpp"
#include "NotificationService.hpp"

namespace domain {

class TicketService {
private:
    ITicketRepository& tRepo;
    ICustomerRepository& cRepo;
    NotificationService& notifier;
    std::shared_ptr<ILogger> logger;

    std::unique_ptr<AbstractTicketFactory> factory;

    int ticketCounter = 1000;

public:
    TicketService(
        ITicketRepository& t,
        ICustomerRepository& c,
        NotificationService& n,
        std::shared_ptr<ILogger> l,
        std::unique_ptr<AbstractTicketFactory> f
    )
        : tRepo(t), cRepo(c), notifier(n), logger(l), factory(std::move(f)) {}

    std::string createTicket(
        const std::string& customerId,
        const std::string& description,
        Priority priority,
        TicketCategory category = TicketCategory::GENERAL
    ) {
        auto customer = cRepo.findById(customerId);

        if (!customer) {
            logger->log("Failed to create ticket: Customer not found: " + customerId);
            return "";
        }

        std::string id = "TKT-" + std::to_string(++ticketCounter);

        auto ticket = factory->createTicket(
            id,
            customerId,
            description,
            priority,
            category
        );

        tRepo.save(*ticket);

        logger->log("Ticket created: " + id + " ("
            + TicketFactory::getCategoryName(category) + ", "
            + TicketFactory::getPriorityName(priority) + ")");

        notifier.notify(
            customer->getEmail(),
            "Your ticket " + id + " has been created."
        );

        return id;
    }

    bool updateTicketStatus(const std::string& id, TicketStatus newStatus) {
        auto ticket = tRepo.findById(id);
        if (!ticket) {
            logger->log("Ticket not found: " + id);
            return false;
        }

        ticket->setStatus(newStatus);
        tRepo.save(*ticket);

        auto customer = cRepo.findById(ticket->getCustomerId());
        if (customer) {
            notifier.notify(
                customer->getEmail(),
                "Ticket " + id + " status updated to: " +
                    TicketFactory::getStatusName(newStatus)
            );
        }

        logger->log("Ticket updated: " + id);
        return true;
    }

    std::vector<std::shared_ptr<Ticket>> getAllTickets() {
        return tRepo.findAll();
    }
};

} // namespace domain

#endif
