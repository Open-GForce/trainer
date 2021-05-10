#include "AccelerationService.hpp"

using namespace GForce::Processing::Acceleration;

AccelerationService::AccelerationService()
{
    this->stages = {};
    this->softStartAcceleration = 1000;
    this->softStartSpeed = 10;
    this->accelerationMode = AccelerationMode::targetSpeed;
}

int AccelerationService::getAcceleration(double currentSpeed, double targetSpeed)
{
    if (currentSpeed <= this->softStartSpeed) {
        return this->softStartAcceleration;
    }

    double needle = 0;

    switch (this->accelerationMode) {
        case AccelerationMode::differential:
            needle = std::abs(targetSpeed - currentSpeed); break;
        case AccelerationMode::targetSpeed:
            needle = targetSpeed; break;
        case AccelerationMode::currentSpeed:
        default:
            needle = currentSpeed; break;
    }

    std::list<AccelerationStage>:: reverse_iterator revit;
    for( revit = this->stages.rbegin(); revit != this->stages.rend(); revit++) {
        if (revit->getSpeed() <= needle) {
            return revit->getAcceleration();
        }
    }

    return this->softStartAcceleration;
}

void AccelerationService::setStages(const std::list<AccelerationStage> &newStages)
{
    this->stages = newStages;
    this->stages.sort();
}

void AccelerationService::setSoftStartSpeed(double speed)
{
    this->softStartSpeed = speed;
}

void AccelerationService::setSoftStartAcceleration(int value)
{
    this->softStartAcceleration = value;
}

void AccelerationService::setAccelerationMode(AccelerationMode mode) {
    this->accelerationMode = mode;
}

