#pragma once

#include <memory>
#include "../../models/Enums.hpp"
#include "ITicketState.hpp"
#include "TicketStateMachine.hpp"

namespace domain::behaviors::state {

// ----------------------------------------------------------
// OPEN STATE
// ----------------------------------------------------------
class OpenState : public ITicketState {
public:
    TicketStatus getStatus() const override {
        return TicketStatus::OPEN;
    }

    void startProgress(TicketStateMachine& ctx) override;
    void resolve(TicketStateMachine& /*ctx*/) override { }
    void close(TicketStateMachine& /*ctx*/) override { }
    void reopen(TicketStateMachine& /*ctx*/) override { }
};

// ----------------------------------------------------------
// IN PROGRESS STATE
// ----------------------------------------------------------
class InProgressState : public ITicketState {
public:
    TicketStatus getStatus() const override {
        return TicketStatus::IN_PROGRESS;
    }

    void startProgress(TicketStateMachine& /*ctx*/) override { }
    void resolve(TicketStateMachine& ctx) override;
    void close(TicketStateMachine& ctx) override;
    void reopen(TicketStateMachine& /*ctx*/) override { }
};

// ----------------------------------------------------------
// RESOLVED STATE
// ----------------------------------------------------------
class ResolvedState : public ITicketState {
public:
    TicketStatus getStatus() const override {
        return TicketStatus::RESOLVED;
    }

    void startProgress(TicketStateMachine& /*ctx*/) override { }
    void resolve(TicketStateMachine& /*ctx*/) override { }
    void close(TicketStateMachine& ctx) override;
    void reopen(TicketStateMachine& ctx) override;
};

// ----------------------------------------------------------
// CLOSED STATE
// ----------------------------------------------------------
class ClosedState : public ITicketState {
public:
    TicketStatus getStatus() const override {
        return TicketStatus::CLOSED;
    }

    void startProgress(TicketStateMachine& /*ctx*/) override { }
    void resolve(TicketStateMachine& /*ctx*/) override { }
    void close(TicketStateMachine& /*ctx*/) override { }
    void reopen(TicketStateMachine& ctx) override;
};


// ----------------------------------------------------------
// IMPLEMENTATIONS
// (Must appear AFTER all classes are declared)
// ----------------------------------------------------------

// OpenState → InProgress
inline void OpenState::startProgress(TicketStateMachine& ctx) {
    ctx.setState(std::make_shared<InProgressState>());
}

// InProgress → Resolved
inline void InProgressState::resolve(TicketStateMachine& ctx) {
    ctx.setState(std::make_shared<ResolvedState>());
}

// InProgress → Closed
inline void InProgressState::close(TicketStateMachine& ctx) {
    ctx.setState(std::make_shared<ClosedState>());
}

// Resolved → Closed
inline void ResolvedState::close(TicketStateMachine& ctx) {
    ctx.setState(std::make_shared<ClosedState>());
}

// Resolved → Open
inline void ResolvedState::reopen(TicketStateMachine& ctx) {
    ctx.setState(std::make_shared<OpenState>());
}

// Closed → Open
inline void ClosedState::reopen(TicketStateMachine& ctx) {
    ctx.setState(std::make_shared<OpenState>());
}

} // namespace domain::behaviors::state
