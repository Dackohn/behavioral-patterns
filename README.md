# Behavioral Design Patterns

### *Implementation Report*

This report describes the implementation of three behavioral design patterns inside the **Support Ticket Management System**:

* **Chain of Responsibility**
* **State**
* **Template Method** (optional but included as a complementary behavioral pattern)

Each pattern provides a different behavioral mechanism that improves flexibility, extensibility, and decoupling inside the system.

---

## 1. Chain of Responsibility Pattern

### **Purpose**

The **Chain of Responsibility (CoR)** pattern allows a request to be processed by a pipeline of handlers, where each handler may approve, reject, or pass the request further.
It decouples request senders from receivers and supports dynamic extension of validation logic.

### **Why CoR fits the system**

Ticket creation in any support system must go through **multiple validation steps**, such as:

* verifying the customer exists
* checking the description validity
* validating priority rules

Hard-coding all validation steps inside `TicketService` would create a monolithic and rigid function.

CoR allows each validation rule to be:

* independent
* reusable
* replaceable
* easily extendable
* executed in sequence only if previous ones succeed

---

## **Implementation Summary**

### **Core classes**

The CoR implementation consists of:

| File                            | Description                                          |
| ------------------------------- | ---------------------------------------------------- |
| `ITicketHandler.hpp`            | Abstract handler defining `handle()` and `process()` |
| `TicketCreationRequest.hpp`     | DTO for collecting validation info                   |
| `CustomerExistsHandler.hpp`     | Validates customer existence                         |
| `DescriptionLengthHandler.hpp`  | Ensures description has a minimum length             |
| `PriorityValidationHandler.hpp` | Validates rules tied to high/critical priority       |

### **Chain Creation**

The CLI constructs a validation pipeline:

```cpp
auto customerHandler = std::make_shared<CustomerExistsHandler>(*customerService);
auto lengthHandler   = std::make_shared<DescriptionLengthHandler>(10);
auto priorityHandler = std::make_shared<PriorityValidationHandler>();

customerHandler->setNext(lengthHandler);
lengthHandler->setNext(priorityHandler);

customerHandler->handle(req);
```

### **Result of the Chain**

* If any handler sets `req.valid = false`, the entire chain stops.
* The user receives detailed feedback.
* If all handlers approve the request, the ticket is created.

### **Benefits Provided**

* modular validation
* no duplicated code
* extensible pipeline
* each validation rule is isolated

---

## 2. State Pattern

### **Purpose**

The **State Pattern** allows an object to change its behavior dynamically when its internal state changes.
Instead of using `if/else` or `switch` statements to represent ticket status transitions, each state becomes its own class.

### **Why State fits the system**

Support tickets naturally move through well-defined stages:

```
OPEN → IN_PROGRESS → RESOLVED → CLOSED
           ↘ reopen ↗         ↖ reopen
```

Embedding these rules directly in `TicketService` becomes difficult as logic grows.

The State pattern provides:

* encapsulated transition logic
* clear, maintainable state machines
* ability to change or extend statuses without modifying existing code
* elimination of complex conditionals

---

## **Implementation Summary**

### **Core Classes:**

| File                     | Description                                                      |
| ------------------------ | ---------------------------------------------------------------- |
| `ITicketState.hpp`       | Base interface for all states                                    |
| `TicketStateMachine.hpp` | Holds the current state and transitions                          |
| `TicketStates.hpp`       | Contains: OpenState, InProgressState, ResolvedState, ClosedState |

Each state implements allowed transitions:

Example:

```cpp
void InProgressState::resolve(TicketStateMachine& ctx) {
    ctx.setState(std::make_shared<ResolvedState>());
}
```

### **State Machine Usage**

The CLI provides:

```cpp
auto state = std::make_shared<OpenState>();
TicketStateMachine sm(state);

sm.startProgress();
sm.resolve();
sm.close();
sm.reopen();
```

### **Integration with Ticket Model**

`Ticket.hpp` contains:

```cpp
void applyStateMachine(TicketStateMachine& sm) {
    status = sm.getStatus();
}
```

This allows the state machine to drive real system logic.

### **Benefits Provided**

* separation of concerns
* prevents invalid state transitions
* easy to modify workflow logic
* matches real-world ticket lifecycle behavior

---

## 3. Template Method Pattern

### **Purpose**

The Template Method defines the **skeleton of an algorithm**, while allowing concrete steps to be overridden by subclasses.

### **Why Template Method fits the system**

Although not exposed directly in user-facing code, the system internally uses a **template-like workflow** for ticket creation inside factories and builders:

* All tickets must be constructed with:

  * an ID
  * customer reference
  * description
  * metadata (priority, category)
* Additional optional metadata (tags, assignment, timestamps) follow a standard build process.

This build sequence is fixed, but certain steps (tagging, assignment) can differ for different ticket types or future extensions.

### **Implementation Summary**

#### **TicketBuilder.hpp**

```cpp
auto ticket = std::make_shared<Ticket>(id, customerId, description, priority, category);
if (!assignedTo.empty()) ticket->setAssignedTo(assignedTo);
for (const auto& t : tags) ticket->addTag(t);
```

This acts as template method:

1. **Create ticket base**
2. **Apply optional assignments**
3. **Apply optional tags**

The algorithm is fixed, but the optional steps are variable.

### **Benefits Provided**

* consistent building process
* flexible extension of ticket creation rules
* avoids repeating logic in services or factories
* supports future specialized ticket types (billing ticket, security ticket, etc.)

---

## Conclusion

The integration of these three behavioral patterns greatly improves the maintainability and flexibility of the support ticket system.

| Pattern                 | Problem Solved                    | Benefit                           |
| ----------------------- | --------------------------------- | --------------------------------- |
| Chain of Responsibility | Complex, growing validation rules | Modular validation pipeline       |
| State                   | Hard-coded ticket workflow logic  | Clean, scalable state transitions |
| Template Method         | Repeated construction steps       | Extensible building workflow      |

As the system grows—new ticket types, complex validation, workflow automation—these behavioral patterns ensure the architecture remains:

* maintainable
* extensible
* modular
* aligned with real-world support systems
