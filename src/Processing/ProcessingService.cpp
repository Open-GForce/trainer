#include <iostream>
#include "ProcessingService.hpp"

using namespace GForce::Processing;

ProcessingService::ProcessingService(MoviDriveService* driveService, UserSettings* settings)
{
    this->driveService = driveService;
    this->status = nullptr;

    this->released = false;
    this->maxSpeed = 0;

    this->innerBrake = 0;
    this->outerBrake = 0;

    this->innerBrakeRange = nullptr;
    this->outerBrakeRange = nullptr;
    this->loadUserConfig(settings);
    this->direction = RotationDirection::right;
}

ProcessingService::~ProcessingService()
{
    delete this->innerBrakeRange;
    delete this->outerBrakeRange;
}

void ProcessingService::run()
{
    if (!this->released) {
        return this->sync(ControlStatus::softBrake(), 0);
    }

    double speed = this->calcTargetSpeed();
    this->sync(ControlStatus::release(), speed);
}

double ProcessingService::calcTargetSpeed()
{
    double innerValue = this->innerBrakeRange->getLimitedPercentage(this->innerBrake);
    double outerValue = this->outerBrakeRange->getLimitedPercentage(this->outerBrake);

    double delta = innerValue - outerValue;
    return delta * this->maxSpeed;
}

void ProcessingService::sync(ControlStatus *controlStatus, double rotationSpeed)
{
    this->driveService->setControlStatus(controlStatus);
    this->driveService->setRotationSpeed(rotationSpeed);

    auto response = this->driveService->sync();
    if (response != nullptr) {
        this->statusMutex.lock();
        delete this->status;
        this->status = response;
        this->statusMutex.unlock();
    }
}

void ProcessingService::setFirstBrakeInput(int input)
{
    if (this->direction == RotationDirection::right) {
        this->innerBrake = input;
    } else {
        this->outerBrake = input;
    }
}

void ProcessingService::setSecondBrakeInput(int input)
{
    if (this->direction == RotationDirection::right) {
        this->outerBrake = input;
    } else {
        this->innerBrake = input;
    }
}

ProcessingStatus *ProcessingService::getStatus()
{
    return new ProcessingStatus(
            this->status != nullptr ? this->status->getEngineStatus()->clone() : nullptr,
            this->status != nullptr ? this->status->getRotationSpeed() : -1,
            this->maxSpeed,
            this->calcTargetSpeed(),
            this->innerBrake,
            this->outerBrake,
            this->innerBrakeRange->getLimitedPercentage(this->innerBrake),
            this->outerBrakeRange->getLimitedPercentage(this->outerBrake),
            this->direction
    );
}

BusResponse* ProcessingService::cloneStatus()
{
    this->statusMutex.lock();
    auto cloned = this->status != nullptr ? this->status->clone() : nullptr;
    this->statusMutex.unlock();
    return cloned;
}

void ProcessingService::loadUserConfig(UserSettings *settings)
{
    delete this->innerBrakeRange;
    delete this->outerBrakeRange;

    this->innerBrakeRange = settings->getInnerBrakeRange()->clone();
    this->outerBrakeRange = settings->getOuterBrakeRange()->clone();
}

void ProcessingService::setReleased(bool isReleased) {
    ProcessingService::released = isReleased;
}

void ProcessingService::setMaxSpeed(double speed) {
    ProcessingService::maxSpeed = speed;
}

void ProcessingService::setDirection(RotationDirection value) {
    ProcessingService::direction = value;
}




