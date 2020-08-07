#include "ConfigurationController.hpp"
#include "../../Utils/Assertions/Assertion.hpp"

using namespace GForce::Utils::Assertions;
using namespace GForce::API;

Controller::ConfigurationController::ConfigurationController(ProcessingThread *processingThread, ConfigRepository *configRepository) :
    processingThread(processingThread), configRepository(configRepository) {}

UserSettings* Controller::ConfigurationController::getUserSettings()
{
    return this->configRepository->loadUserSettings();
}

void Controller::ConfigurationController::setInnerBrakeRange(Request *request)
{
    auto range = buildRange(request);

    auto oldConfig = this->configRepository->loadUserSettings();
    auto newConfig = new UserSettings(
          range,
          oldConfig->getOuterBrakeRange()->clone(),
          oldConfig->getRotationRadius(),
          oldConfig->getSoftStartSpeed(),
          oldConfig->getSoftStartAcceleration(),
          oldConfig->getAccelerationStages());

    this->saveConfig(newConfig, oldConfig);
}

void Controller::ConfigurationController::setOuterBrakeRange(Request *request)
{
    auto range = buildRange(request);

    auto oldConfig = this->configRepository->loadUserSettings();
    auto newConfig = new UserSettings(
            oldConfig->getInnerBrakeRange()->clone(),
            range,
            oldConfig->getRotationRadius(),
            oldConfig->getSoftStartSpeed(),
            oldConfig->getSoftStartAcceleration(),
            oldConfig->getAccelerationStages());

    this->saveConfig(newConfig, oldConfig);
}

void Controller::ConfigurationController::setRotationRadius(Request *request)
{
    Assertion::jsonExistsAndNumber(request->getData(), "rotationRadius");

    auto oldConfig = this->configRepository->loadUserSettings();
    auto newConfig = new UserSettings(oldConfig->getInnerBrakeRange()->clone(),
                                      oldConfig->getOuterBrakeRange()->clone(),
                                      request->getData()["rotationRadius"],
                                      oldConfig->getSoftStartSpeed(),
                                      oldConfig->getSoftStartAcceleration(),
                                      oldConfig->getAccelerationStages());

    this->saveConfig(newConfig, oldConfig);
}

Range* Controller::ConfigurationController::buildRange(Request *request)
{
    Assertion::jsonExistsAndNumber(request->getData(), "min");
    Assertion::jsonExistsAndNumber(request->getData(), "max");

    return new Range(request->getData()["min"], request->getData()["max"]);
}

void Controller::ConfigurationController::saveConfig(UserSettings *newConfig, UserSettings *oldConfig)
{
    this->configRepository->saveUserSettings(newConfig);
    this->processingThread->reloadUserConfig(newConfig);

    delete oldConfig;
    delete newConfig;
}
