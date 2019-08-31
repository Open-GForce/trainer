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

    this->innerBrakeRange = settings->getInnerBrakeRange()->clone();
    this->outerBrakeRange = settings->getOuterBrakeRange()->clone();

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

    double innerValue = this->innerBrakeRange->getLimitedPercentage(this->innerBrake);
    double outerValue = this->outerBrakeRange->getLimitedPercentage(this->outerBrake);

    double delta = innerValue - outerValue;
    double speed = delta * this->maxSpeed;

    this->sync(ControlStatus::release(), speed);
}

void ProcessingService::sync(ControlStatus *controlStatus, double rotationSpeed)
{
    this->driveService->setControlStatus(controlStatus);
    this->driveService->setRotationSpeed(rotationSpeed);

    auto response = this->driveService->sync();
    if (response != nullptr) {
        delete this->status;
        this->status = response;
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

Response *ProcessingService::getStatus() const {
    return status;
}

void ProcessingService::setReleased(bool isReleased) {
    ProcessingService::released = isReleased;
}

void ProcessingService::setMaxSpeed(int speed) {
    ProcessingService::maxSpeed = speed;
}

void ProcessingService::setDirection(RotationDirection value) {
    ProcessingService::direction = value;
}




