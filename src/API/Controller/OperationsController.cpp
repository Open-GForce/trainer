#include "OperationsController.hpp"
#include "../../Utils/Assertions/Assertion.hpp"
#include "../../Utils/Assertions/AssertionFailedException.hpp"
#include "../../Processing/Mode/RegularSpiralMode.hpp"
#include "../../Processing/Mode/StaticMaxSpeedMode.hpp"
#include "HeartbeatResponse.hpp"

using namespace GForce::Utils::Assertions;
using namespace GForce::API::Controller;
using namespace GForce::Processing;

OperationsController::OperationsController(ProcessingThread *processingThread)
{
    this->processingThread = processingThread;
}

void OperationsController::handleSpeedLimit(Request *request)
{
    Assertion::jsonExistsAndNumber(request->getData(), "speed");

    double maxSpeed = request->getData()["speed"];
    this->processingThread->setMaxSpeed(maxSpeed);
}

void OperationsController::handleRotationDirection(Request *request)
{
    Assertion::jsonExistsAndString(request->getData(), "direction");

    auto direction = request->getData()["direction"] == "right"
            ? RotationDirection::right
            : RotationDirection::left;

    this->processingThread->setDirection(direction);
}

void OperationsController::handleReleaseStatus(Request *request)
{
    Assertion::jsonExistsAndBool(request->getData(), "released");
    this->processingThread->setReleased(request->getData()["released"]);
}

void Controller::OperationsController::handleOperationMode(Request *request)
{
    Assertion::jsonExistsAndString(request->getData(), "mode");
    std::string mode = request->getData()["mode"];

    if (mode == Mode::RegularSpiralMode::IDENTIFIER) {
        this->processingThread->setOperationMode(new RegularSpiralMode());
        return;
    }

    if (mode == Mode::StaticMaxSpeedMode::IDENTIFIER) {
        this->processingThread->setOperationMode(new StaticMaxSpeedMode());
        return;
    }

    throw AssertionFailedException("Invalid mode identifier given");
}

HeartbeatResponse* Controller::OperationsController::handleHeartbeat()
{
    return new HeartbeatResponse();
}
