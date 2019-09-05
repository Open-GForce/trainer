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
