#ifndef CHAT_NOTIFICATION_ADAPTER_HPP
#define CHAT_NOTIFICATION_ADAPTER_HPP

#include <memory>
#include <string>

#include "../../domain/interfaces/INotificationChannel.hpp"
#include "ExternalChatAPI.hpp"

namespace infrastructure {

// Adapter: adapts ExternalChatAPI to INotificationChannel
class ChatNotificationAdapter : public domain::INotificationChannel {
private:
    std::shared_ptr<external::ExternalChatAPI> api;

public:
    ChatNotificationAdapter()
        : api(std::make_shared<external::ExternalChatAPI>()) {}

    bool send(const std::string& recipient,
              const std::string& message) override {
        // Interpret 'recipient' as chat channel ID
        api->postToChannel(recipient, message);
        return true;
    }

    std::string getChannelName() const override {
        return "Chat (Adapter)";
    }
};

} // namespace infrastructure

#endif
