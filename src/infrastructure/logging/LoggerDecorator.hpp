#ifndef LOGGER_DECORATOR_HPP
#define LOGGER_DECORATOR_HPP

#include <memory>
#include <string>
#include "../../domain/interfaces/ILogger.hpp"

namespace infrastructure {

class LoggerDecorator : public domain::ILogger {
protected:
    std::shared_ptr<domain::ILogger> inner;

public:
    explicit LoggerDecorator(std::shared_ptr<domain::ILogger> logger)
        : inner(std::move(logger)) {}

    void log(const std::string& message) override {
        if (inner) {
            inner->log(message);
        }
    }
};

} // namespace infrastructure

#endif
