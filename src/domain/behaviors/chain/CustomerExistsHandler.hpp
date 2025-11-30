#pragma once

#include <memory>
#include <string>

#include "ITicketHandler.hpp"
#include "../../services/CustomerService.hpp"

namespace domain::behaviors::chain {

class CustomerExistsHandler : public ITicketHandler {
private:
    domain::CustomerService& customerService;

public:
    explicit CustomerExistsHandler(domain::CustomerService& service)
        : customerService(service) {}

protected:
    void process(TicketCreationRequest& request) override {
        auto customer = customerService.getCustomer(request.customerId);
        if (!customer) {
            request.valid = false;
            request.errorMessage = "Customer does not exist: " + request.customerId;
        }
    }
};

} // namespace domain::behaviors::chain
