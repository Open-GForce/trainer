#ifndef GFORCE_TRAINER_CONTROLLER_MESSAGEINTERFACE_HPP
#define GFORCE_TRAINER_CONTROLLER_MESSAGEINTERFACE_HPP

#include <string>

namespace GForce::ACL::CAN {

class MessageInterface
{
    public:
        virtual ~MessageInterface() = default;

        /**
         * Encodes message to socket compatible format (e.g. binary, ASCII, etc.)
         */
        virtual std::string toFrame() = 0;

        /**
         * Get CAN ID
         */
        virtual int getIndex() = 0;

        /**
         * Get number of data bytes (max. 8)
         */
        virtual int getSize() = 0;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_MESSAGEINTERFACE_HPP
