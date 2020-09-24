#include <json.hpp>
#include "../../Utils/Assertions/Assertion.hpp"
#include "../../Utils/Exceptions/RuntimeException.hpp"
#include "RequestRouter.hpp"

using namespace GForce::Utils::Assertions;
using namespace GForce::Utils::Exceptions;
using namespace GForce::API::Websocket;

RequestRouter::RequestRouter(OperationsController *operationsController, ConfigurationController *configurationController) :
    operationsController(operationsController), configurationController(configurationController) {}

ResponseCastInterface* RequestRouter::handle(const std::string& message)
{
    nlohmann::json data = nlohmann::json::parse(message);

    Assertion::jsonExistsAndString(data, "type");
    Assertion::jsonExistsAndObject(data, "data");

    auto request = new Request(data["type"], data["data"]);
    try {
        auto response = this->route(request);
        delete request;
        return response;
    } catch (std::exception &e) {
        delete request;
        throw e;
    }
}

ResponseCastInterface* RequestRouter::route(Request *request)
{
    if (request->getType() == RequestRouter::TYPE_SET_MAX_SPEED) {
        this->operationsController->handleSpeedLimit(request);
        return nullptr;
    }

    if (request->getType() == RequestRouter::TYPE_SET_DIRECTION) {
        this->operationsController->handleRotationDirection(request);
        return nullptr;
    }

    if (request->getType() == RequestRouter::TYPE_SET_RELEASE) {
        this->operationsController->handleReleaseStatus(request);
        return nullptr;
    }

    if (request->getType() == RequestRouter::TYPE_SET_OPERATION_MODE) {
        this->operationsController->handleOperationMode(request);
        return nullptr;
    }

    if (request->getType() == RequestRouter::TYPE_SET_CONFIG_INNER_BRAKE) {
        this->configurationController->setInnerBrakeRange(request);
        return nullptr;
    }

    if (request->getType() == RequestRouter::TYPE_SET_CONFIG_OUTER_BRAKE) {
        this->configurationController->setOuterBrakeRange(request);
        return nullptr;
    }

    if (request->getType() == RequestRouter::TYPE_SET_CONFIG_ROT_RADIUS) {
        this->configurationController->setRotationRadius(request);
        return nullptr;
    }

    if (request->getType() == RequestRouter::TYPE_SET_CONFIG_SOFT_START) {
        this->configurationController->setSoftStart(request);
        return nullptr;
    }

    if (request->getType() == RequestRouter::TYPE_SET_CONFIG_ACC_STAGES) {
        this->configurationController->setAccelerationStages(request);
        return nullptr;
    }

    if (request->getType() == RequestRouter::TYPE_SET_CONFIG_UI) {
        this->configurationController->setUserInterfaceSettings(request);
        return nullptr;
    }

    if (request->getType() == RequestRouter::TYPE_GET_USER_CONFIG) {
        return this->configurationController->getUserSettings();
    }

    if (request->getType() == RequestRouter::TYPE_HEARTBEAT) {
        return this->operationsController->handleHeartbeat();
    }

    throw RuntimeException("No route defined for " + request->getType());
}
