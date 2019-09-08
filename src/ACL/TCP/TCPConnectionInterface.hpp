#ifndef GFORCE_TRAINER_CONTROLLER_TCPCONNECTIONINTERFACE_HPP
#define GFORCE_TRAINER_CONTROLLER_TCPCONNECTIONINTERFACE_HPP

#include <string>

namespace GForce::ACL::TCP {

class TCPConnectionInterface
{
    public:
        /**
         * Sends a message to the socket with \n appended
         */
        virtual void send(const std::string& data) = 0;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_TCPCONNECTIONINTERFACE_HPP
