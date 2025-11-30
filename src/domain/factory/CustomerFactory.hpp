#ifndef CUSTOMER_FACTORY_HPP
#define CUSTOMER_FACTORY_HPP

#include <memory>
#include <string>

#include "../models/Customer.hpp"
#include "../models/Enums.hpp"
#include "../builder/CustomerBuilder.hpp"

namespace domain {

// Abstract Creator
class AbstractCustomerFactory {
public:
    virtual ~AbstractCustomerFactory() = default;

    virtual std::shared_ptr<Customer> createCustomer(
        const std::string& id,
        const std::string& name,
        const std::string& email,
        const std::string& phone,
        CustomerType type
    ) = 0; 
};

// Concrete Creator
class CustomerFactory : public AbstractCustomerFactory {
public:
    std::shared_ptr<Customer> createCustomer(
        const std::string& id,
        const std::string& name,
        const std::string& email,
        const std::string& phone,
        CustomerType type
    ) override
    {
        std::string finalName = getTypePrefix(type) + name;

        return CustomerBuilder()
            .withId(id)
            .withName(finalName)
            .withEmail(email)
            .withPhone(phone)
            .withType(type)
            .build();
    }

    static std::string getTypePrefix(CustomerType t) {
        switch(t) {
            case CustomerType::PREMIUM: return "[PREMIUM] ";
            case CustomerType::VIP:     return "[VIP] ";
            default:                    return "";
        }
    }

    static std::string getTypeName(CustomerType t) {
        switch(t) {
            case CustomerType::PREMIUM: return "Premium";
            case CustomerType::VIP:     return "VIP";
            default:                    return "Regular";
        }
    }
};

} // namespace domain

#endif
