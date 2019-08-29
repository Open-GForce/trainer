#ifndef GFORCE_TRAINER_CONTROLLER_MESSAGEFACTORY_HPP
#define GFORCE_TRAINER_CONTROLLER_MESSAGEFACTORY_HPP

#include <vector>
#include "MessageInterface.hpp"

namespace GForce::ACL::CAN
{

class MessageFactory
{
    private:
        /**
         * Splits stream into frames without padding (<  >)
         */
        static std::vector<std::string> split(const std::string& data);

        /**
         * Decodes a single frame to message
         */
        static MessageInterface* decode(const std::string& frame);

        /**
         * Converts ASCII hex to int
         */
        static int hexToString(const std::string& value);

    public:
        /**
         * Decodes a ASCII string to CAN messages
         */
        static std::vector<MessageInterface*> createFromString(const std::string& data);
};

}

#endif //GFORCE_TRAINER_CONTROLLER_MESSAGEFACTORY_HPP
