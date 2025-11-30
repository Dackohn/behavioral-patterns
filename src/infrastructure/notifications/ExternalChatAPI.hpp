#ifndef EXTERNAL_CHAT_API_HPP
#define EXTERNAL_CHAT_API_HPP

#include <iostream>
#include <string>

namespace external {

class ExternalChatAPI {
public:
    void postToChannel(const std::string& channelId,
                       const std::string& text) {
        std::cout << "[ExternalChatAPI] Posting to channel "
                  << channelId << ":\n"
                  << text << std::endl;
    }
};

} // namespace external

#endif
