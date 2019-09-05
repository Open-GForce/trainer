#ifndef GFORCE_TRAINER_CONTROLLER_REQUESTROUTER_HPP
#define GFORCE_TRAINER_CONTROLLER_REQUESTROUTER_HPP

#include "Request.hpp"
#include "RouterInterface.hpp"
#include "../Controller/OperationsController.hpp"
#include "../Controller/ConfigurationController.hpp"

using namespace GForce::API::Controller;

namespace GForce::API::Websocket {

class RequestRouter : public RouterInterface
{
    const std::string TYPE_SET_MAX_SPEED = "setMaxSpeed";
    const std::string TYPE_SET_DIRECTION = "setRotationDirection";
    const std::string TYPE_SET_RELEASE   = "setReleaseStatus";

    const std::string TYPE_SET_CONFIG_INNER_BRAKE = "setInnerBrakeRange";
    const std::string TYPE_SET_CONFIG_OUTER_BRAKE = "setOuterBrakeRange";

    private:
        OperationsController* operationsController;
        ConfigurationController* configurationController;

        void route(Request* request);

    public:
        explicit RequestRouter(OperationsController *operationsController);

        RequestRouter(OperationsController *operationsController, ConfigurationController *configurationController);

        /**
         * Decodes and routes incoming messages
         */
        void handle(const std::string& message) override;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_REQUESTROUTER_HPP
