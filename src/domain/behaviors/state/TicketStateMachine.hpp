#pragma once

#include <memory>
#include "../../models/Enums.hpp"
#include "ITicketState.hpp"

namespace domain::behaviors::state {

class TicketStateMachine {
private:
    std::shared_ptr<ITicketState> current;

public:
    explicit TicketStateMachine(std::shared_ptr<ITicketState> initial)
        : current(std::move(initial)) {}

    TicketStatus getStatus() const {
        return current->getStatus();
    }

    void setState(std::shared_ptr<ITicketState> newState) {
        current = std::move(newState);
    }

    void startProgress() { current->startProgress(*this); }
    void resolve()       { current->resolve(*this); }
    void close()         { current->close(*this); }
    void reopen()        { current->reopen(*this); }
};

} // namespace domain::behaviors::state
