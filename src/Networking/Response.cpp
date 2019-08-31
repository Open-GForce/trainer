#include "Response.hpp"

using namespace GForce::Networking;

Response::Response(GForce::Networking::EngineStatus *engineStatus, double rotationSpeed)
        : engineStatus(engineStatus), rotationSpeed(rotationSpeed) {}


Response::~Response() {
    delete this->engineStatus;
}

Response* Response::fromMessage(CAN::MessageInterface* message)
{
    auto statusByte = message->getData()[0];

    auto status = new EngineStatus(
            statusByte & (1 << 0),
            statusByte & (1 << 1),
            statusByte & (1 << 2),
            ((statusByte & (1 << 3)) ? 2 : 1),
            ((statusByte & (1 << 4)) ? 2 : 1),
            statusByte & (1 << 5),
            statusByte & (1 << 6),
            statusByte & (1 << 7)
    );

    double speed = message->getData()[2] | (message->getData()[3] << 8);

    // 1 digit = 0.2 min^-1
    speed /= 5;

    return new Response(status, speed);
}

EngineStatus *Response::getEngineStatus() const {
    return engineStatus;
}

double Response::getRotationSpeed() const {
    return rotationSpeed;
}
