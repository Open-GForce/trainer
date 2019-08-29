#ifndef GFORCE_TRAINER_CONTROLLER_SOCKETINTERFACE_HPP
#define GFORCE_TRAINER_CONTROLLER_SOCKETINTERFACE_HPP

#include <vector>
#include "MessageInterface.hpp"

namespace GForce::ACL::CAN {

class SocketInterface
{
    public:
        virtual ~SocketInterface() = default;

        /**
        * Sends a CAN message
        * Ownership of message object moves to socket
        */
        virtual void send(MessageInterface* message) = 0;

        /**
        * Receives messages in pipe
        */
        virtual std::vector<MessageInterface*> receive() = 0;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_SOCKETINTERFACE_HPP
