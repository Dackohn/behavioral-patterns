#include "CLI.hpp"
#include <iostream>

namespace client {

void CommandLineInterface::showMenu() {
    std::cout << "\n========= SUPPORT SYSTEM =========\n";
    std::cout << "1. Register customer\n";
    std::cout << "2. Create ticket\n";
    std::cout << "3. Register customer + create ticket (Facade)\n";
    std::cout << "4. List notification channels (Adapter visible)\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose: ";
}

void CommandLineInterface::run() {
    int choice = -1;
    while (choice != 0) {
        showMenu();
        std::cin >> choice;

        switch (choice) {
        case 1:
            handleCreateCustomer();
            break;
        case 2:
            handleCreateTicket();
            break;
        case 3:
            handleFacadeWorkflow();
            break;
        case 4:
            handleListNotificationChannels();
            break;
        case 0:
            break;
        default:
            std::cout << "Invalid option.\n";
        }
    }
}

void CommandLineInterface::handleCreateCustomer() {
    std::string name, email, phone;

    std::cout << "Customer name: ";
    std::cin >> name;
    std::cout << "Email: ";
    std::cin >> email;
    std::cout << "Phone: ";
    std::cin >> phone;

    customerService->registerCustomer(name, email, phone);
}

void CommandLineInterface::handleCreateTicket() {
    std::string customerId, issue;
    int priorityInt, categoryInt;

    std::cout << "Customer ID: ";
    std::cin >> customerId;

    std::cout << "Issue: ";
    std::cin.ignore();
    std::getline(std::cin, issue);

    std::cout << "Priority (0=LOW,1=MEDIUM,2=HIGH,3=CRITICAL): ";
    std::cin >> priorityInt;

    std::cout << "Category (0=TECHNICAL,1=BILLING,2=GENERAL,3=COMPLAINT,4=FEATURE_REQUEST): ";
    std::cin >> categoryInt;

    auto priority = static_cast<domain::Priority>(priorityInt);
    auto category = static_cast<domain::TicketCategory>(categoryInt);

    ticketService->createTicket(customerId, issue, priority, category);
}

// =======================
// Facade demo
// =======================
void CommandLineInterface::handleFacadeWorkflow() {
    std::string name, email, phone, issue;
    int priorityInt, categoryInt;

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
    std::cin >> priorityInt;

    std::cout << "Category (0=TECHNICAL,1=BILLING,2=GENERAL,3=COMPLAINT,4=FEATURE_REQUEST): ";
    std::cin >> categoryInt;

    auto priority = static_cast<domain::Priority>(priorityInt);
    auto category = static_cast<domain::TicketCategory>(categoryInt);

    auto [customerId, ticketId] = facade.registerCustomerAndOpenTicket(
        name,
        email,
        phone,
        issue,
        priority,
        category
    );

    std::cout << "Customer ID: " << customerId << "\n";
    std::cout << "Ticket ID: " << ticketId << "\n";
}

// =======================
// Adapter demo
// =======================
void CommandLineInterface::handleListNotificationChannels() {
    std::cout << "\nRegistered notification channels:\n";

    for (const auto& c : notifier.getChannels()) {
        if (c) {
            std::cout << " - " << c->getChannelName() << "\n";
        }
    }
}

} // namespace client
