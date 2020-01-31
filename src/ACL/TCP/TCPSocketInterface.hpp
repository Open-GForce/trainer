#ifndef GFORCE_TRAINER_CONTROLLER_TCPSOCKETINTERFACE_HPP
#define GFORCE_TRAINER_CONTROLLER_TCPSOCKETINTERFACE_HPP

#include <string>

namespace GForce::ACL::TCP {

class TCPSocketInterface {
    public:
        virtual ~TCPSocketInterface() = default;

        /**
         * Reads next \n seperated message in pipe
         */
        virtual std::string read() = 0;

        /**
         * Closes the socket
         *
         * @throw May throw implementation specific exceptions
         */
        virtual void close() = 0;
};

class TCPSocketFactory {
    public:
        virtual ~TCPSocketFactory() = default;

        /**
         * Listens for new connections on the given port, waits until connection received
         */
        virtual TCPSocketInterface* listen(int port) = 0;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_TCPSOCKETINTERFACE_HPP
