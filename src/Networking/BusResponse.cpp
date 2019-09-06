#include "BusResponse.hpp"
#include "MoviDriveService.hpp"

using namespace GForce::Networking;

BusResponse::BusResponse(GForce::Networking::EngineStatus *engineStatus, double rotationSpeed)
        : engineStatus(engineStatus), rotationSpeed(rotationSpeed) {}


BusResponse::~BusResponse() {
    delete this->engineStatus;
}

BusResponse* BusResponse::fromMessage(CAN::MessageInterface* message)
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

    speed /= SPEED_SCALE_FACTOR;

    return new BusResponse(status, speed);
}

BusResponse *BusResponse::clone()
{
    return new BusResponse(this->engineStatus->clone(), this->rotationSpeed);
}

EngineStatus *BusResponse::getEngineStatus() const {
    return engineStatus;
}

double BusResponse::getRotationSpeed() const {
    return rotationSpeed;
}
