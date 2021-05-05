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

SystemSettings *Controller::ConfigurationController::getSystemSettings()
{
    return this->configRepository->loadSystemSettings();
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
            oldConfig->getAccelerationStages(),
            oldConfig->getAccelerationMode(),
            oldConfig->isAdaptiveAccelerationUIActivated(),
            oldConfig->isOuterBrakeDeactivated());

    this->saveConfig(newConfig, oldConfig);
}

void Controller::ConfigurationController::setOuterBrakeRange(Request *request)
{
    Assertion::jsonExistsAndBool(request->getData(), "deactivated");
    auto range = buildRange(request);

    auto oldConfig = this->configRepository->loadUserSettings();
    auto newConfig = new UserSettings(
            oldConfig->getInnerBrakeRange()->clone(),
            range,
            oldConfig->getRotationRadius(),
            oldConfig->getSoftStartSpeed(),
            oldConfig->getSoftStartAcceleration(),
            oldConfig->getAccelerationStages(),
            oldConfig->getAccelerationMode(),
            oldConfig->isAdaptiveAccelerationUIActivated(),
            request->getData()["deactivated"]);

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
                                      oldConfig->getAccelerationStages(),
                                      oldConfig->getAccelerationMode(),
                                      oldConfig->isAdaptiveAccelerationUIActivated(),
                                      oldConfig->isOuterBrakeDeactivated());

    this->saveConfig(newConfig, oldConfig);
}


void Controller::ConfigurationController::setSoftStart(Request *request)
{
    Assertion::jsonExistsAndNumber(request->getData(), "speed");
    Assertion::jsonExistsAndNumber(request->getData(), "acceleration");

    auto oldConfig = this->configRepository->loadUserSettings();
    auto newConfig = new UserSettings(oldConfig->getInnerBrakeRange()->clone(),
                                      oldConfig->getOuterBrakeRange()->clone(),
                                      oldConfig->getRotationRadius(),
                                      request->getData()["speed"],
                                      request->getData()["acceleration"],
                                      oldConfig->getAccelerationStages(),
                                      oldConfig->getAccelerationMode(),
                                      oldConfig->isAdaptiveAccelerationUIActivated(),
                                      oldConfig->isOuterBrakeDeactivated());

    this->saveConfig(newConfig, oldConfig);
}

void Controller::ConfigurationController::setAccelerationStages(Request *request)
{
    Assertion::jsonExistsAndArray(request->getData(), "stages");
    Assertion::jsonExistsAndString(request->getData(), "mode");

    std::list<AccelerationStage> stages = {};

    for (auto& item : request->getData()["stages"]) {
        Assertion::jsonExistsAndNumber(item, "speed");
        Assertion::jsonExistsAndNumber(item, "acceleration");

        stages.push_back(AccelerationStage(item["speed"], item["acceleration"]));
    }

    auto oldConfig = this->configRepository->loadUserSettings();
    auto newConfig = new UserSettings(oldConfig->getInnerBrakeRange()->clone(),
                                      oldConfig->getOuterBrakeRange()->clone(),
                                      oldConfig->getRotationRadius(),
                                      oldConfig->getSoftStartSpeed(),
                                      oldConfig->getSoftStartAcceleration(),
                                      stages,
                                      UserSettings::stringToAccelerationMode(request->getData()["mode"]),
                                      oldConfig->isAdaptiveAccelerationUIActivated(),
                                      oldConfig->isOuterBrakeDeactivated());

    this->saveConfig(newConfig, oldConfig);
}

void Controller::ConfigurationController::setUserInterfaceSettings(Request* request)
{
    Assertion::jsonExistsAndBool(request->getData(), "activateAdaptiveAcceleration");

    auto oldConfig = this->configRepository->loadUserSettings();
    auto newConfig = new UserSettings(oldConfig->getInnerBrakeRange()->clone(),
                                      oldConfig->getOuterBrakeRange()->clone(),
                                      oldConfig->getRotationRadius(),
                                      oldConfig->getSoftStartSpeed(),
                                      oldConfig->getSoftStartAcceleration(),
                                      oldConfig->getAccelerationStages(),
                                      oldConfig->getAccelerationMode(),
                                      request->getData()["activateAdaptiveAcceleration"],
                                      oldConfig->isOuterBrakeDeactivated());

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
