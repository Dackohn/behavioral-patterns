#include <memory>

// CLIENT
#include "client/CLI.hpp"

// DOMAIN SERVICES
#include "domain/services/CustomerService.hpp"
#include "domain/services/TicketService.hpp"
#include "domain/services/NotificationService.hpp"
#include "domain/services/SupportFacade.hpp"

// FACTORIES
#include "domain/factory/CustomerFactory.hpp"
#include "domain/factory/TicketFactory.hpp"

// INFRASTRUCTURE
#include "infrastructure/logging/ConsoleLogger.hpp"
#include "infrastructure/logging/TimestampLogger.hpp"
#include "infrastructure/repositories/InMemoryCustomerRepository.hpp"
#include "infrastructure/repositories/InMemoryTicketRepository.hpp"
#include "infrastructure/notifications/EmailNotification.hpp"
#include "infrastructure/notifications/SMSNotification.hpp"
#include "infrastructure/notifications/PushNotification.hpp"
#include "infrastructure/notifications/ChatNotificationAdapter.hpp"

int main() {
    // LOGGER (Decorator)
    auto baseLogger = std::make_shared<infrastructure::ConsoleLogger>();
    auto logger     = std::make_shared<infrastructure::TimestampLogger>(baseLogger);

    // REPOSITORIES
    auto& customerRepo = infrastructure::InMemoryCustomerRepository::getInstance();
    auto& ticketRepo   = infrastructure::InMemoryTicketRepository::getInstance();

    // NOTIFICATION SERVICE (Singleton-ish)
    auto& notifier = domain::NotificationService::getInstance(logger);
    notifier.addChannel(std::make_shared<infrastructure::EmailNotification>());
    notifier.addChannel(std::make_shared<infrastructure::SMSNotification>());
    notifier.addChannel(std::make_shared<infrastructure::PushNotification>());
    notifier.addChannel(std::make_shared<infrastructure::ChatNotificationAdapter>()); // Adapter

    // SERVICES
    auto customerService = std::make_shared<domain::CustomerService>(
        customerRepo,
        logger,
        std::make_unique<domain::CustomerFactory>()
    );

    auto ticketService = std::make_shared<domain::TicketService>(
        ticketRepo,
        customerRepo,
        notifier,
        logger,
        std::make_unique<domain::TicketFactory>()
    );

    // FACADE
    domain::SupportFacade facade(customerService, ticketService, notifier);

    // CLI
    client::CommandLineInterface cli(customerService, ticketService, facade, notifier);
    cli.run();

    return 0;
}
