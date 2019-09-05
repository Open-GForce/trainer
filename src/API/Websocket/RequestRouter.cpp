#include <json.hpp>
#include "../../Utils/Assertions/Assertion.hpp"
#include "../../Utils/Exceptions/RuntimeException.hpp"
#include "RequestRouter.hpp"

using namespace GForce::Utils::Assertions;
using namespace GForce::Utils::Exceptions;
using namespace GForce::API::Websocket;

RequestRouter::RequestRouter(OperationsController *operationsController) : operationsController(operationsController) {}

void RequestRouter::handle(const std::string& message)
{
    nlohmann::json data = nlohmann::json::parse(message);

    Assertion::jsonExistsAndString(data, "type");
    Assertion::jsonExistsAndObject(data, "data");

    auto request = new Request(data["type"], data["data"]);
    try {
        this->route(request);
    } catch (std::exception &e) {
        delete request;
        throw e;
    }
}

void RequestRouter::route(Request *request)
{
    if (request->getType() == RequestRouter::TYPE_SET_MAX_SPEED) {
        return this->operationsController->handleSpeedLimit(request);
    }

    if (request->getType() == RequestRouter::TYPE_SET_DIRECTION) {
        return this->operationsController->handleRotationDirection(request);
    }

    if (request->getType() == RequestRouter::TYPE_SET_RELEASE) {
        return this->operationsController->handleReleaseStatus(request);
    }

    throw RuntimeException("No route defined for " + request->getType());
}
