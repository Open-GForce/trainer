#ifndef GFORCE_TRAINER_CONTROLLER_RESPONSECASTINTERFACE_HPP
#define GFORCE_TRAINER_CONTROLLER_RESPONSECASTINTERFACE_HPP

#include "Response.hpp"

namespace GForce::API::Websocket {

class ResponseCastInterface
{
    public:
        virtual ~ResponseCastInterface() = default;

        /**
         * Ownership of returned response moves to caller
         */
        virtual Response* toResponse() = 0;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_RESPONSECASTINTERFACE_HPP
