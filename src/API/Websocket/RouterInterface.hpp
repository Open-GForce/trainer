#ifndef GFORCE_TRAINER_CONTROLLER_ROUTERINTERFACE_HPP
#define GFORCE_TRAINER_CONTROLLER_ROUTERINTERFACE_HPP

#include <string>

namespace GForce::API::Websocket {

class RouterInterface
{
    public:
         /**
         * Decodes and routes incoming messages
         */
         virtual void handle(const std::string& message) = 0;
};

}


#endif //GFORCE_TRAINER_CONTROLLER_ROUTERINTERFACE_HPP
