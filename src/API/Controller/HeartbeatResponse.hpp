#ifndef GFORCE_TRAINER_CONTROLLER_HEARTBEATRESPONSE_HPP
#define GFORCE_TRAINER_CONTROLLER_HEARTBEATRESPONSE_HPP

#include "../Websocket/ResponseCastInterface.hpp"

using namespace GForce::API::Websocket;

namespace GForce::API::Controller {

class HeartbeatResponse : public ResponseCastInterface
{
    public:
        Response *toResponse() override {
            return new Response("heartbeat", {});
        }
};

}


#endif //GFORCE_TRAINER_CONTROLLER_HEARTBEATRESPONSE_HPP
