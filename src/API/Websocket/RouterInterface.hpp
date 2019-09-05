#ifndef GFORCE_TRAINER_CONTROLLER_ROUTERINTERFACE_HPP
#define GFORCE_TRAINER_CONTROLLER_ROUTERINTERFACE_HPP

#include <string>
#include "ResponseCastInterface.hpp"

namespace GForce::API::Websocket {

class RouterInterface
{
    public:
         /**
         * Decodes and routes incoming messages
          *
          * May return a response (ownership moves to caller)
         */
         virtual ResponseCastInterface* handle(const std::string& message) = 0;
};

}


#endif //GFORCE_TRAINER_CONTROLLER_ROUTERINTERFACE_HPP
