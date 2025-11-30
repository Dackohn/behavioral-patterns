# Structural Design Patterns

## **Patterns Implemented:**

* Decorator
* Adapter
* Facade

---

## 1. Introduction

This project extends an existing support ticket management system with **three classic structural design patterns**. These patterns increase flexibility, improve extensibility, and decouple components without modifying the existing business logic.

The system initially contained:

* Customer and ticket models
* Repositories (in-memory)
* Logging
* Notification channels (Email, SMS, Push)
* CustomerService and TicketService
* A simple CLI interface

To fulfill the laboratory requirements, three structural patterns were added:

1. **Decorator Pattern** — Enhancing the logging mechanism
2. **Adapter Pattern** — Integrating an external chat notification API
3. **Facade Pattern** — Simplifying common workflows for clients

The following sections describe each pattern, their purpose, implementation details, and usage inside the project.

---

# 2. Decorator Pattern

## 2.1. Intent

The **Decorator pattern** allows behavior to be added to an object dynamically, without modifying its original class.
This is used when we want to wrap an existing object with additional responsibilities.

**Why here?**
The system already had a `ConsoleLogger` implementing `ILogger`. We wanted:

* to add timestamps to log messages,
* **without modifying ConsoleLogger**,
* and without changing any existing service that uses ILogger.

The Decorator perfectly fits this use case.

---

## 2.2. Implementation Overview

### **Component Interface**

```cpp
class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const std::string& message) = 0;
};
```

### **Concrete Component**

```cpp
class ConsoleLogger : public ILogger {
public:
    void log(const std::string& msg) override;
};
```

### **Decorator Base Class**

```cpp
class LoggerDecorator : public ILogger {
protected:
    std::shared_ptr<ILogger> inner;
public:
    explicit LoggerDecorator(std::shared_ptr<ILogger> logger);
    void log(const std::string& msg) override;
};
```

### **Concrete Decorator: TimestampLogger**

```cpp
class TimestampLogger : public LoggerDecorator {
public:
    TimestampLogger(std::shared_ptr<ILogger> logger);
    void log(const std::string& message) override;
};
```

### **Decorator Usage (main.cpp)**

```cpp
auto baseLogger = std::make_shared<ConsoleLogger>();
auto logger     = std::make_shared<TimestampLogger>(baseLogger);
```

All domain services now automatically use timestamped logging **without any code changes to services or loggers**.

---

## 2.3. Benefits in This Project

* Logging improved without modifying existing components
* Multiple decorators could be easily chained (file logger, color logger, etc.)
* Services remain unaware of logging enhancements
* Open for extension, closed for modification (SOLID: OCP)

---

# 3. Adapter Pattern

## 3.1. Intent

The **Adapter pattern** converts the interface of a class into another interface clients expect.

This is used when:

* We need to integrate a **third-party** component
* The third-party API **does not follow our internal interface**
* Changing existing interfaces is undesirable

**Why here?**
The system has its own `INotificationChannel` interface.
We added a simulated 3rd-party chat API:

```cpp
class ExternalChatAPI {
public:
    void postToChannel(const std::string& channelId, const std::string& text);
};
```

This API is incompatible with the internal notification system.

---

## 3.2. Implementation Overview

### **Target Interface**

```cpp
class INotificationChannel {
public:
    virtual bool send(const std::string& recipient,
                      const std::string& message) = 0;
    virtual std::string getChannelName() const = 0;
};
```

### **Adaptee (External API)**

```cpp
class ExternalChatAPI {
public:
    void postToChannel(const std::string& channelId,
                       const std::string& text);
};
```

### **Adapter**

```cpp
class ChatNotificationAdapter : public INotificationChannel {
private:
    std::shared_ptr<ExternalChatAPI> api;

public:
    ChatNotificationAdapter();
    bool send(const std::string& recipient, const std::string& msg) override;
    std::string getChannelName() const override;
};
```

### **Adapter Usage**

The adapter is added into the notification system:

```cpp
notifier.addChannel(std::make_shared<ChatNotificationAdapter>());
```

Now the system treats the external API just like any other notification channel (Email, SMS, Push), without any code changes to existing components.

---

## 3.3. Benefits in This Project

* Clean integration of an external API
* NotificationService needs no modifications
* Follows the “program to interface” principle
* New alternative channels can be easily added (Slack, Discord, WhatsApp…)

---

# 4. Facade Pattern

## 4.1. Intent

The **Facade pattern** provides a simplified API over a set of complex subsystems.

In our system, registering a customer and opening a support ticket requires interacting with:

* `CustomerService`
* `TicketService`
* `NotificationService`

A CLI or UI should **not** need to know this complexity.

---

## 4.2. Implementation Overview

### **Facade Class**

```cpp
class SupportFacade {
private:
    std::shared_ptr<CustomerService> customerService;
    std::shared_ptr<TicketService> ticketService;
    NotificationService& notifier;

public:
    std::pair<std::string, std::string> registerCustomerAndOpenTicket(
        const std::string& name,
        const std::string& email,
        const std::string& phone,
        const std::string& issueDescription,
        Priority priority,
        TicketCategory category
    );
};
```

### **Workflow Provided**

1. Register customer
2. Create ticket
3. Notify customer

The CLI only calls:

```cpp
facade.registerCustomerAndOpenTicket(...);
```

instead of interacting with three different services.

---

## 4.3. Usage Example (CLI)

Menu option:

```
3. Register customer + create ticket (Facade)
```

Triggers:

```cpp
auto [customerId, ticketId] = facade.registerCustomerAndOpenTicket(...);
```

---

## 4.4. Benefits for This Project

* Simplifies CLI logic
* Encapsulates multi-step workflows in one place
* Supports future UI layers (web, desktop, REST API)
* Reduces coupling between client code and business logic

---

# 5. Summary Table

| Pattern       | Purpose                                         | Implementation Location   | System Benefit                                      |
| ------------- | ----------------------------------------------- | ------------------------- | --------------------------------------------------- |
| **Decorator** | Add responsibilities dynamically                | `TimestampLogger`         | Improved logging without modifying services         |
| **Adapter**   | Convert external API to internal interface      | `ChatNotificationAdapter` | Seamless integration of external chat notifications |
| **Facade**    | Simplify subsystem usage with unified interface | `SupportFacade`           | Cleaner, simpler CLI & easier workflows             |

---

# 6. Conclusion

By implementing **three structural design patterns**, the system became:

* more modular
* easier to extend
* easier to integrate with external systems
* decoupled and aligned with SOLID principles

These patterns demonstrate clear, practical improvements in real-world software architecture and provide a strong foundation for future enhancements such as GUI/REST APIs, additional notification channels, or advanced logging systems.
