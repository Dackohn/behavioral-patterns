#pragma once

#include "../../models/Enums.hpp"

namespace domain::behaviors::state {

class TicketStateMachine;

class ITicketState {
public:
    virtual ~ITicketState() = default;

    virtual TicketStatus getStatus() const = 0;

    virtual void startProgress(TicketStateMachine& ctx) = 0;
    virtual void resolve(TicketStateMachine& ctx) = 0;
    virtual void close(TicketStateMachine& ctx) = 0;
    virtual void reopen(TicketStateMachine& ctx) = 0;
};

} // namespace domain::behaviors::state
