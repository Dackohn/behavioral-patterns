#pragma once

#include "ITicketHandler.hpp"

namespace domain::behaviors::chain {

class DescriptionLengthHandler : public ITicketHandler {
private:
    std::size_t minLength;

public:
    explicit DescriptionLengthHandler(std::size_t minLen = 10)
        : minLength(minLen) {}

protected:
    void process(TicketCreationRequest& request) override {
        if (request.description.size() < minLength) {
            request.valid = false;
            request.errorMessage =
                "Description too short. Min length: " + std::to_string(minLength);
        }
    }
};

} // namespace domain::behaviors::chain
