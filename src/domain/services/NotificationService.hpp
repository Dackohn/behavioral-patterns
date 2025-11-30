#ifndef NOTIFICATION_SERVICE_HPP
#define NOTIFICATION_SERVICE_HPP

#include <memory>
#include <string>
#include <vector>

#include "../interfaces/INotificationChannel.hpp"
#include "../interfaces/ILogger.hpp"

namespace domain {

class NotificationService {
private:
    std::vector<std::shared_ptr<INotificationChannel>> channels;
    std::shared_ptr<ILogger> logger;

    explicit NotificationService(std::shared_ptr<ILogger> log)
        : logger(std::move(log)) {}

public:
    NotificationService(const NotificationService&) = delete;
    NotificationService& operator=(const NotificationService&) = delete;

    static NotificationService& getInstance(std::shared_ptr<ILogger> logger) {
        static NotificationService instance(std::move(logger));
        return instance;
    }

    void addChannel(std::shared_ptr<INotificationChannel> channel) {
        channels.push_back(std::move(channel));
    }

    const std::vector<std::shared_ptr<INotificationChannel>>& getChannels() const {
        return channels;
    }

    void notify(const std::string& recipient, const std::string& message) {
        for (auto& channel : channels) {
            if (!channel) continue;

            bool ok = channel->send(recipient, message);
            if (logger) {
                if (ok) {
                    logger->log("Notification sent via " + channel->getChannelName() +
                                " to " + recipient);
                } else {
                    logger->log("Notification FAILED via " + channel->getChannelName() +
                                " to " + recipient);
                }
            }
        }
    }
};

} // namespace domain

#endif
