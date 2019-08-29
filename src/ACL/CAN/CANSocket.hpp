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
        int handle;

        void transfer(const std::string& data);
        std::string fetch();

    public:
        CANSocket();

        /**
         * Connects to the CAN socket daemon
         */
        void connect(const std::string& address, uint16_t port);

        /**
         * Opens can0 and switches to BCM mode
         */
        void open();

        /**
         * Sends a CAN message
         * Ownership of message object moves to socket
         */
        void send(MessageInterface* message) override;

        /**
         * Receives messages in pipe
         */
        std::vector<MessageInterface*> receive();
};

}

#endif //GFORCE_TRAINER_CONTROLLER_CANSOCKET_HPP
