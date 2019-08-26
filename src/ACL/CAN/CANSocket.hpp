#ifndef GFORCE_TRAINER_CONTROLLER_CANSOCKET_HPP
#define GFORCE_TRAINER_CONTROLLER_CANSOCKET_HPP

#include <net/if.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "Message.hpp"

namespace GForce::ACL::CAN {

class CANSocket
{
    private:
        int rawSocket;
        sockaddr_can socketCANAddress;
        ifreq frequency;

    public:
        void open();
        void send(Message* message);
};

}

#endif //GFORCE_TRAINER_CONTROLLER_CANSOCKET_HPP
