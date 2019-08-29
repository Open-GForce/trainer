#ifndef GFORCE_TRAINER_CONTROLLER_CANSOCKET_HPP
#define GFORCE_TRAINER_CONTROLLER_CANSOCKET_HPP

#include <net/if.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "MessageInterface.hpp"
#include "SocketInterface.hpp"

namespace GForce::ACL::CAN {

class CANSocket : public SocketInterface
{
    private:
        int rawSocket;
        sockaddr_can socketCANAddress;
        ifreq frequency;

    public:
        void open();

        /**
         * Sends a CAN message
         * Ownership of message object moves to socket
         */
        void send(MessageInterface* message) override;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_CANSOCKET_HPP
