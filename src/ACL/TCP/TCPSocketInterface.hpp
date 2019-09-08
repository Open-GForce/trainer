#ifndef GFORCE_TRAINER_CONTROLLER_TCPSOCKETINTERFACE_HPP
#define GFORCE_TRAINER_CONTROLLER_TCPSOCKETINTERFACE_HPP

#include <string>

namespace GForce::ACL::TCP {

class TCPSocketInterface {
    public:
        /**
         * Reads next \n seperated message in pipe
         */
        virtual std::string read() = 0;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_TCPSOCKETINTERFACE_HPP
