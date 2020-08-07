#include "ProcessingService.hpp"
#include "Mode/RegularSpiralMode.hpp"

using namespace GForce::Processing;

ProcessingService::ProcessingService(MoviDriveService *driveService, UserSettings *settings,
                                     AccelerationService *accelerationService)
        : accelerationService(accelerationService) {
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

    this->operationMode = new RegularSpiralMode();
}

ProcessingService::~ProcessingService()
{
    delete this->innerBrakeRange;
    delete this->outerBrakeRange;
    delete this->operationMode;
}

void ProcessingService::init()
{
    this->driveService->startNode();
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

    double speed = this->operationMode->getTargetSpeed(innerValue, outerValue);
    speed = speed * this->maxSpeed;

    // Direction is determined via sign of speed
    return this->direction == RotationDirection::right
           ? speed
           : 0 - speed;
}

void ProcessingService::sync(ControlStatus *controlStatus, double rotationSpeed)
{
    this->driveService->setControlStatus(controlStatus);
    this->driveService->setRotationSpeed(rotationSpeed);

    double currentSpeed = this->status != nullptr ? this->status->getRotationSpeed() : 0;
    int acceleration = this->accelerationService->getAcceleration(currentSpeed, rotationSpeed);

    this->driveService->setAcceleration(acceleration);

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
    if (this->direction == RotationDirection::left) {
        this->innerBrake = input;
    } else {
        this->outerBrake = input;
    }
}

void ProcessingService::setSecondBrakeInput(int input)
{
    if (this->direction == RotationDirection::left) {
        this->outerBrake = input;
    } else {
        this->innerBrake = input;
    }
}

ProcessingStatus *ProcessingService::getStatus()
{
    double rotationSpeed = std::abs(this->status->getRotationSpeed());

    return new ProcessingStatus(
            this->status != nullptr ? this->status->getEngineStatus()->clone() : nullptr,
            this->status != nullptr ? rotationSpeed : -1,
            this->maxSpeed,
            this->calcTargetSpeed(),
            this->innerBrake,
            this->outerBrake,
            this->innerBrakeRange->getLimitedPercentage(this->innerBrake),
            this->outerBrakeRange->getLimitedPercentage(this->outerBrake),
            this->direction,
            this->operationMode->getIdentifier());
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

    this->accelerationService->setSoftStartAcceleration(settings->getSoftStartAcceleration());
    this->accelerationService->setSoftStartSpeed(settings->getSoftStartSpeed());
    this->accelerationService->setStages(settings->getAccelerationStages());
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

void ProcessingService::setOperationMode(OperationMode *mode) {
    delete this->operationMode;
    this->operationMode = mode;
}