#ifndef TIMESTAMP_LOGGER_HPP
#define TIMESTAMP_LOGGER_HPP

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#include "LoggerDecorator.hpp"

namespace infrastructure {

class TimestampLogger : public LoggerDecorator {
public:
    explicit TimestampLogger(std::shared_ptr<domain::ILogger> logger)
        : LoggerDecorator(std::move(logger)) {}

    void log(const std::string& message) override {
        auto now   = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm tmVal{};

#if defined(_WIN32)
        localtime_s(&tmVal, &t);
#else
        localtime_r(&t, &tmVal);
#endif

        std::ostringstream oss;
        oss << "[" << std::put_time(&tmVal, "%Y-%m-%d %H:%M:%S") << "] ";

        if (inner) {
            inner->log(oss.str() + message);
        }
    }
};

} // namespace infrastructure

#endif
