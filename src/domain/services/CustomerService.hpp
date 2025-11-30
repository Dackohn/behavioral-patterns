#ifndef CUSTOMER_SERVICE_HPP
#define CUSTOMER_SERVICE_HPP

#include <memory>
#include <string>
#include <vector>

#include "../models/Customer.hpp"
#include "../models/Enums.hpp"
#include "../interfaces/ILogger.hpp"
#include "../interfaces/ICustomerRepository.hpp"
#include "../factory/CustomerFactory.hpp"

namespace domain {

class CustomerService {
private:
    ICustomerRepository& repo;
    std::shared_ptr<ILogger> logger;
    std::unique_ptr<AbstractCustomerFactory> factory;

    int customerCounter = 1000;

public:
    CustomerService(
        ICustomerRepository& r,
        std::shared_ptr<ILogger> l,
        std::unique_ptr<AbstractCustomerFactory> f
    )
        : repo(r), logger(l), factory(std::move(f)) {}

    std::string registerCustomer(
        const std::string& name,
        const std::string& email,
        const std::string& phone,
        CustomerType type = CustomerType::REGULAR
    ) {
        std::string id = "CUST-" + std::to_string(++customerCounter);

        auto customer = factory->createCustomer(
            id,
            name,
            email,
            phone,
            type
        );

        repo.save(*customer);

        logger->log("Customer registered: " + id +
            " (Type: " + CustomerFactory::getTypeName(type) + ")");

        return id;
    }

    std::shared_ptr<Customer> getCustomer(const std::string& id) {
        return repo.findById(id);
    }

    std::vector<std::shared_ptr<Customer>> getAllCustomers() {
        return repo.findAll();
    }
};

} // namespace domain

#endif
