#include "OperationsController.hpp"
#include "../../Utils/Assertions/Assertion.hpp"

using namespace GForce::Utils::Assertions;
using namespace GForce::API::Controller;

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
