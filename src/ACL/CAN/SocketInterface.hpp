#ifndef GFORCE_TRAINER_CONTROLLER_SOCKETINTERFACE_HPP
#define GFORCE_TRAINER_CONTROLLER_SOCKETINTERFACE_HPP

#include "MessageInterface.hpp"

namespace GForce::ACL::CAN {

class SocketInterface
{
    public:
        virtual ~SocketInterface() = default;

        virtual void send(MessageInterface* message) = 0;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_SOCKETINTERFACE_HPP
