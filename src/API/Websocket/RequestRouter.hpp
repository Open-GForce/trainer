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
    const std::string TYPE_SET_MAX_SPEED      = "setMaxSpeed";
    const std::string TYPE_SET_DIRECTION      = "setRotationDirection";
    const std::string TYPE_SET_RELEASE        = "setReleaseStatus";
    const std::string TYPE_SET_OPERATION_MODE = "setOperationMode";

    const std::string TYPE_GET_USER_CONFIG         = "getUserSettings";
    const std::string TYPE_GET_ACTIVE_CONFIG_NAME  = "getActiveUserSettingsName";
    const std::string TYPE_CREATE_USER_CONFIG      = "createUserSettings";
    const std::string TYPE_DELETE_USER_CONFIG      = "deleteUserSettings";
    const std::string TYPE_SWITCH_USER_CONFIG      = "switchUserSettings";
    const std::string TYPE_GET_SYSTEM_CONFIG       = "getSystemSettings";
    const std::string TYPE_SET_CONFIG_INNER_BRAKE  = "setInnerBrakeRange";
    const std::string TYPE_SET_CONFIG_OUTER_BRAKE  = "setOuterBrakeRange";
    const std::string TYPE_SET_CONFIG_ROT_RADIUS   = "setRotationRadius";
    const std::string TYPE_SET_CONFIG_SOFT_START   = "setSoftStart";
    const std::string TYPE_SET_CONFIG_ACC_STAGES   = "setAccelerationStages";
    const std::string TYPE_SET_CONFIG_UI           = "setUserInterfaceSettings";

    const std::string TYPE_HEARTBEAT = "heartbeat";

    private:
        OperationsController* operationsController;
        ConfigurationController* configurationController;

        ResponseCastInterface* route(Request* request);

    public:
        RequestRouter(OperationsController *operationsController, ConfigurationController *configurationController);

        /**
         * Decodes and routes incoming messages
         */
        ResponseCastInterface* handle(const std::string& message) override;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_REQUESTROUTER_HPP
