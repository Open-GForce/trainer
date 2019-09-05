#ifndef GFORCE_TRAINER_CONTROLLER_REQUESTROUTER_HPP
#define GFORCE_TRAINER_CONTROLLER_REQUESTROUTER_HPP

#include "Request.hpp"
#include "RouterInterface.hpp"
#include "../Controller/OperationsController.hpp"

using namespace GForce::API::Controller;

namespace GForce::API::Websocket {

class RequestRouter : public RouterInterface
{
    const std::string TYPE_SET_MAX_SPEED = "setMaxSpeed";

    private:
        OperationsController* operationsController;

        void route(Request* request);

    public:
        explicit RequestRouter(OperationsController *operationsController);

        /**
         * Decodes and routes incoming messages
         */
        void handle(const std::string& message) override;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_REQUESTROUTER_HPP
