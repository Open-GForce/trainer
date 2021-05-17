#include <nlohmann/json.hpp>
#include "../../Utils/Assertions/Assertion.hpp"
#include "../../Utils/Exceptions/RuntimeException.hpp"
#include "RequestRouter.hpp"

using namespace GForce::Utils::Assertions;
using namespace GForce::Utils::Exceptions;
using namespace GForce::API::Websocket;

RequestRouter::RequestRouter(LoggerInterface *logger, OperationsController *operationsController,
                             ConfigurationController *configurationController) :
        operationsController(operationsController), configurationController(configurationController), logger(logger) {}

ResponseCastInterface* RequestRouter::handle(const std::string& message)
{
    nlohmann::json data = nlohmann::json::parse(message);

    Assertion::jsonExistsAndString(data, "type");
    Assertion::jsonExistsAndObject(data, "data");

    auto request = new Request(data["type"], data["data"]);
    this->logger->info("Websocket", "New " + request->getType() + " request. Message = " + message, {
            {"type", request->getType()},
            {"data", request->getData()}
    });

    try {
        auto response = this->route(request);
        delete request;
        return response;
    } catch (std::exception &e) {
        this->logger->error("Websocket", "Error handling request => " + std::string(e.what()), {
                {"type", request->getType()},
                {"data", request->getData()}
        });

        delete request;
        throw;
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
        return this->configurationController->getUserSettings(request);
    }

    if (request->getType() == RequestRouter::TYPE_GET_ACTIVE_CONFIG_NAME) {
        return this->configurationController->getActiveConfigurationName();
    }

    if (request->getType() == RequestRouter::TYPE_GET_AVAILABLE_CONFIG_NAMES) {
        return this->configurationController->getAvailableUserSettings();
    }

    if (request->getType() == RequestRouter::TYPE_CREATE_USER_CONFIG) {
        this->configurationController->createUserSettings(request);
        return nullptr;
    }

    if (request->getType() == RequestRouter::TYPE_DELETE_USER_CONFIG) {
        this->configurationController->deleteUserSettings(request);
        return nullptr;
    }

    if (request->getType() == RequestRouter::TYPE_SWITCH_USER_CONFIG) {
        this->configurationController->switchUserSettings(request);
        return nullptr;
    }

    if (request->getType() == RequestRouter::TYPE_GET_SYSTEM_CONFIG) {
        return this->configurationController->getSystemSettings();
    }

    if (request->getType() == RequestRouter::TYPE_HEARTBEAT) {
        return this->operationsController->handleHeartbeat();
    }

    throw RuntimeException("No route defined for " + request->getType());
}
