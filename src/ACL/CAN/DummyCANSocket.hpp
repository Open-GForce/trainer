#ifndef GFORCE_TRAINER_CONTROLLER_DUMMYCANSOCKET_HPP
#define GFORCE_TRAINER_CONTROLLER_DUMMYCANSOCKET_HPP

#include "SocketInterface.hpp"

namespace GForce::ACL::CAN {

class DummyCANSocket : public SocketInterface
{
    public:
        void send(MessageInterface *message) override;

        std::vector<MessageInterface *> receive() override;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_DUMMYCANSOCKET_HPP
