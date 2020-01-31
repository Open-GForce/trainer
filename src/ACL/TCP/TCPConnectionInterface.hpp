#ifndef GFORCE_TRAINER_CONTROLLER_TCPCONNECTIONINTERFACE_HPP
#define GFORCE_TRAINER_CONTROLLER_TCPCONNECTIONINTERFACE_HPP

#include <string>

namespace GForce::ACL::TCP {

class TCPConnectionInterface
{
    public:
        virtual ~TCPConnectionInterface() = default;

        /**
         * Sends a message to the socket with \n appended
         */
        virtual void send(const std::string& data) = 0;

        /**
         * Closes the socket without graceful shutdown
         *
         * @throws May throw implementation specific exception
         */
        virtual void close() = 0;
};

class TCPConnectionFactory
{
    public:
        virtual ~TCPConnectionFactory() = default;
        virtual TCPConnectionInterface* connect(const std::string& address, int port) = 0;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_TCPCONNECTIONINTERFACE_HPP
