#include <cstring>
#include <algorithm>
#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp>
#include <sstream>
#include "MessageFactory.hpp"
#include "Message.hpp"

using namespace GForce::ACL::CAN;

std::vector<MessageInterface*> MessageFactory::createFromString(const std::string& data)
{
    std::vector<MessageInterface*> messages = {};
    auto frames = MessageFactory::split(data);

    messages.reserve(frames.size());
    for (const auto& frame : frames) {
        messages.push_back(MessageFactory::decode(frame));
    }

    // Filter out null pointer (in case of invalid frames)
    messages.erase(
            std::remove_if(messages.begin(), messages.end(), [] (MessageInterface* message) {
                return message == nullptr;
            }),
            messages.end());

    return messages;
}

std::vector<std::string> MessageFactory::split(const std::string& data)
{
    std::vector<std::string> frames = {};
    std::string buffer;

    for (auto singleChar : data) {
        if (singleChar == '<') {
            buffer = singleChar;
            continue;
        }

        if (!buffer.empty()) {
            buffer += singleChar;
        }

        if (!buffer.empty() && singleChar == '>') {
            // Removing padding
            buffer.erase(0, 2);
            buffer.erase(buffer.size() - 2, 2);

            frames.push_back(buffer);
            buffer = "";
        }
    }

    return frames;
}

/**
 * Typical message: frame 123 23.424242 11223344
 */
MessageInterface *MessageFactory::decode(const std::string& frame)
{
    if (frame.substr(0, 5) != "frame") {
        return nullptr;
    }

    std::vector<std::string> items;
    boost::split(items, frame, boost::is_any_of(" "), boost::token_compress_on);

    int index = hexToString(items[1]);
    std::vector<uint8_t> words = {};

    std::string wordBuffer = "00";

    if (items.size() > 3) {
        for (auto singleChar : items[3]) {
            if (wordBuffer.size() == 2) {
                wordBuffer = singleChar;
            } else {
                wordBuffer += singleChar;
                words.push_back((uint8_t) hexToString(wordBuffer));
            }
        }
    }

    return new Message(index, words);
}

int MessageFactory::hexToString(const std::string& value)
{
    unsigned int result;
    std::stringstream ss;
    ss << std::hex << value;
    ss >> result;

    return result;
}
