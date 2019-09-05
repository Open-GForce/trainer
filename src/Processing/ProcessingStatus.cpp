#include "ProcessingStatus.hpp"

using namespace GForce::Processing;

ProcessingStatus::ProcessingStatus(EngineStatus *engineStatus, double rotationSpeed, double maxSpeed, double targetSpeed,
                                   int innerBrakeRawValue, int outerBrakeRawValue, double innerBrakePercentage,
                                   double outerBrakePercentage, RotationDirection rotationDirection) : engineStatus(
        engineStatus), rotationSpeed(rotationSpeed), maxSpeed(maxSpeed), targetSpeed(targetSpeed), innerBrakeRawValue(
        innerBrakeRawValue), outerBrakeRawValue(outerBrakeRawValue), innerBrakePercentage(innerBrakePercentage),
                                                                                                       outerBrakePercentage(
                                                                                                               outerBrakePercentage),
                                                                                                       rotationDirection(
                                                                                                               rotationDirection) {}

ProcessingStatus::~ProcessingStatus()
{
    delete this->engineStatus;
}

Websocket::Response *ProcessingStatus::toResponse()
{
    nlohmann::json data = {
        {"rotationSpeed", this->rotationSpeed},
        {"maxSpeed", this->maxSpeed},
        {"targetSpeed", this->targetSpeed},
        {"innerBrake", {
           {"raw", this->innerBrakeRawValue},
           {"scaled", this->innerBrakePercentage},
        }},
        {"outerBrake", {
           {"raw", this->outerBrakeRawValue},
           {"scaled", this->outerBrakePercentage},
        }},
        {"rotationDirection", this->rotationDirection == RotationDirection::right ? "right" : "left"}
    };

    if (this->engineStatus == nullptr) {
        data["engineStatus"] = nullptr;
    } else {
        data["engineStatus"] = this->engineStatus->toJSON();
    }

    return new Websocket::Response("processingStatus", data);
}

EngineStatus *ProcessingStatus::getEngineStatus() const {
    return engineStatus;
}

double ProcessingStatus::getRotationSpeed() const {
    return rotationSpeed;
}

double ProcessingStatus::getMaxSpeed() const {
    return maxSpeed;
}

int ProcessingStatus::getInnerBrakeRawValue() const {
    return innerBrakeRawValue;
}

int ProcessingStatus::getOuterBrakeRawValue() const {
    return outerBrakeRawValue;
}

double ProcessingStatus::getInnerBrakePercentage() const {
    return innerBrakePercentage;
}

double ProcessingStatus::getOuterBrakePercentage() const {
    return outerBrakePercentage;
}

double ProcessingStatus::getTargetSpeed() const {
    return targetSpeed;
}

RotationDirection ProcessingStatus::getRotationDirection() const {
    return rotationDirection;
}
