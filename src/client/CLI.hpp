#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// DOMAIN SERVICES
#include "../domain/services/CustomerService.hpp"
#include "../domain/services/TicketService.hpp"
#include "../domain/services/NotificationService.hpp"
#include "../domain/services/SupportFacade.hpp"

// DOMAIN MODELS
#include "../domain/models/Customer.hpp"
#include "../domain/models/Ticket.hpp"
#include "../domain/models/Enums.hpp"

// BEHAVIORAL: CHAIN OF RESPONSIBILITY
#include "../domain/behaviors/chain/TicketCreationRequest.hpp"
#include "../domain/behaviors/chain/ITicketHandler.hpp"
#include "../domain/behaviors/chain/CustomerExistsHandler.hpp"
#include "../domain/behaviors/chain/DescriptionLengthHandler.hpp"
#include "../domain/behaviors/chain/PriorityValidationHandler.hpp"

// BEHAVIORAL: STATE
#include "../domain/behaviors/state/TicketStateMachine.hpp"
#include "../domain/behaviors/state/TicketStates.hpp"

namespace client {

class CommandLineInterface {
private:
    std::shared_ptr<domain::CustomerService> customerService;
    std::shared_ptr<domain::TicketService>   ticketService;
    domain::SupportFacade&                   facade;
    domain::NotificationService&             notifier;

public:
    CommandLineInterface(
        std::shared_ptr<domain::CustomerService> customerService,
        std::shared_ptr<domain::TicketService>   ticketService,
        domain::SupportFacade&                   facade,
        domain::NotificationService&             notifier
    )
        : customerService(std::move(customerService))
        , ticketService(std::move(ticketService))
        , facade(facade)
        , notifier(notifier) {}

    void run() {
        int choice = -1;
        while (choice != 0) {
            showMenu();
            std::cin >> choice;

            switch (choice) {
                case 1: handleCreateCustomer(); break;
                case 2: handleCreateTicketWithValidation(); break; // CoR demo
                case 3: handleFacadeWorkflow(); break;
                case 4: handleListNotificationChannels(); break;
                case 5: handlePrintAllCustomers(); break;
                case 6: handlePrintAllTickets(); break;
                case 7: handleSimulateTicketState(); break;       // State demo
                case 0: break;
                default: std::cout << "Invalid option.\n"; break;
            }
        }
    }

private:
    void showMenu() {
        std::cout << "\n========= SUPPORT SYSTEM =========\n";
        std::cout << "1. Register customer\n";
        std::cout << "2. Create ticket (with validation - Chain of Responsibility)\n";
        std::cout << "3. Register customer + create ticket (Facade)\n";
        std::cout << "4. Show available notification channels (Adapter)\n";
        std::cout << "5. Print all customers\n";
        std::cout << "6. Print all tickets\n";
        std::cout << "7. Simulate ticket lifecycle (State pattern)\n";
        std::cout << "0. Exit\n";
        std::cout << "Choose: ";
    }

    // 1. Register customer
    void handleCreateCustomer() {
        std::string name, email, phone;
        std::cout << "Customer name: ";
        std::cin >> name;
        std::cout << "Email: ";
        std::cin >> email;
        std::cout << "Phone: ";
        std::cin >> phone;

        customerService->registerCustomer(name, email, phone);
    }

    // 2. Create ticket using CoR validation
    void handleCreateTicketWithValidation() {
        using namespace domain::behaviors::chain;

        std::string customerId, issue;
        int priority, category;

        std::cout << "Customer ID: ";
        std::cin >> customerId;
        std::cout << "Issue: ";
        std::cin.ignore();
        std::getline(std::cin, issue);

        std::cout << "Priority (0=LOW,1=MEDIUM,2=HIGH,3=CRITICAL): ";
        std::cin >> priority;

        std::cout << "Category (0=TECHNICAL,1=BILLING,2=GENERAL,3=COMPLAINT,4=FEATURE_REQUEST): ";
        std::cin >> category;

        TicketCreationRequest req{
            customerId,
            issue,
            static_cast<domain::Priority>(priority),
            static_cast<domain::TicketCategory>(category)
        };

        // Build chain: CustomerExists -> DescriptionLength -> PriorityValidation
        auto customerHandler = std::make_shared<CustomerExistsHandler>(*customerService);
        auto lengthHandler   = std::make_shared<DescriptionLengthHandler>(10);
        auto priorityHandler = std::make_shared<PriorityValidationHandler>();

        customerHandler->setNext(lengthHandler);
        lengthHandler->setNext(priorityHandler);

        customerHandler->handle(req);

        if (!req.valid) {
            std::cout << "❌ Ticket creation failed: " << req.errorMessage << "\n";
            return;
        }

        ticketService->createTicket(
            req.customerId,
            req.description,
            req.priority,
            req.category
        );

        std::cout << "✅ Ticket created successfully via validated pipeline.\n";
    }

    // 3. Facade workflow
    void handleFacadeWorkflow() {
        std::string name, email, phone, issue;
        int priority, category;

        std::cout << "Customer name: ";
        std::cin >> name;
        std::cout << "Email: ";
        std::cin >> email;
        std::cout << "Phone: ";
        std::cin >> phone;

        std::cout << "Issue: ";
        std::cin.ignore();
        std::getline(std::cin, issue);

        std::cout << "Priority (0=LOW,1=MEDIUM,2=HIGH,3=CRITICAL): ";
        std::cin >> priority;

        std::cout << "Category (0=TECHNICAL,1=BILLING,2=GENERAL,3=COMPLAINT,4=FEATURE_REQUEST): ";
        std::cin >> category;

        auto result = facade.registerCustomerAndOpenTicket(
            name,
            email,
            phone,
            issue,
            static_cast<domain::Priority>(priority),
            static_cast<domain::TicketCategory>(category)
        );

        std::cout << "Customer ID: " << result.first  << "\n";
        std::cout << "Ticket ID: "   << result.second << "\n";
    }

    // 4. Show channels (no getChannels() dependency)
    void handleListNotificationChannels() {
        std::cout << "\nNotification channels in system (structural pattern demo):\n";
        std::cout << " - EmailNotification\n";
        std::cout << " - SMSNotification\n";
        std::cout << " - PushNotification\n";
        std::cout << " - ChatNotificationAdapter (Adapter)\n";
    }

    // 5. Print customers
    void handlePrintAllCustomers() {
        auto customers = customerService->getAllCustomers();
        std::cout << "\nAll customers:\n";
        for (const auto& c : customers) {
            if (!c) continue;
            std::cout << "ID: "    << c->getId()
                      << " | Name: "  << c->getName()
                      << " | Email: " << c->getEmail()
                      << " | Phone: " << c->getPhone()
                      << "\n";
        }
    }

    // 6. Print tickets
    void handlePrintAllTickets() {
        auto tickets = ticketService->getAllTickets();
        std::cout << "\nAll tickets:\n";
        for (const auto& t : tickets) {
            if (!t) continue;
            std::cout << "ID: "         << t->getId()
                      << " | CustomerID: " << t->getCustomerId()
                      << " | Description: " << t->getDescription()
                      << " | Status: "    << static_cast<int>(t->getStatus())
                      << " | Priority: "  << static_cast<int>(t->getPriority())
                      << " | Category: "  << static_cast<int>(t->getCategory())
                      << "\n";
        }
    }

    // 7. Simulate State pattern
    void handleSimulateTicketState() {
        using namespace domain::behaviors::state;
        using domain::TicketStatus;

        auto state = std::make_shared<OpenState>();
        TicketStateMachine sm(state);

        auto printStatus = [](TicketStatus s) {
            std::cout << "Current status: " << static_cast<int>(s) << "\n";
        };

        std::cout << "\nSimulating ticket lifecycle (State pattern):\n";

        printStatus(sm.getStatus());
        std::cout << "-> startProgress()\n";
        sm.startProgress();
        printStatus(sm.getStatus());

        std::cout << "-> resolve()\n";
        sm.resolve();
        printStatus(sm.getStatus());

        std::cout << "-> close()\n";
        sm.close();
        printStatus(sm.getStatus());

        std::cout << "-> reopen()\n";
        sm.reopen();
        printStatus(sm.getStatus());
    }
};

} // namespace client
