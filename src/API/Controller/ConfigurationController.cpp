#include "ConfigurationController.hpp"
#include "../../Utils/Assertions/Assertion.hpp"

using namespace GForce::Utils::Assertions;
using namespace GForce::API;

Controller::ConfigurationController::ConfigurationController(ProcessingThread *processingThread,
                                                             ConfigRepository *configRepository,
                                                             LoggerInterface *logger) :
        processingThread(processingThread), configRepository(configRepository), currentSettingsName("default"),
        logger(logger) {}

UserSettings* Controller::ConfigurationController::getUserSettings(Request *request)
{
    Assertion::jsonExistsAndString(request->getData(), "name");
    std::string name = request->getData()["name"];
    
    return this->configRepository->loadUserSettings(name);
}

void Controller::ConfigurationController::createUserSettings(Request* request)
{
    Assertion::jsonExistsAndString(request->getData(), "name");
    std::string name = request->getData()["name"];

    if (name.length() > 32) {
        name = name.substr(0, 32);
    }

    auto settings = this->configRepository->loadUserSettings("default");
    this->configRepository->saveUserSettings(name, settings);

    this->logger->info("Configuration", "Created user settings with name = " + name, {});

    delete settings;
}

void Controller::ConfigurationController::switchUserSettings(Request *request)
{
    Assertion::jsonExistsAndString(request->getData(), "name");
    std::string name = request->getData()["name"];

    auto settings = this->configRepository->loadUserSettings(name);
    this->processingThread->reloadUserConfig(settings);

    this->logger->info("Configuration", "Switched user settings to " + name, {});

    this->currentSettingsName = name;
    delete settings;
}

ActiveConfigurationResponse* Controller::ConfigurationController::getActiveConfigurationName()
{
    return new ActiveConfigurationResponse(this->currentSettingsName);
}

AvailableUserSettingsResponse *Controller::ConfigurationController::getAvailableUserSettings()
{
    return new AvailableUserSettingsResponse(this->configRepository->getAvailableUserSettings());
}

void Controller::ConfigurationController::deleteUserSettings(Request *request)
{
    Assertion::jsonExistsAndString(request->getData(), "name");
    std::string name = request->getData()["name"];

    this->configRepository->deleteUserSettings(name);

    if (this->currentSettingsName == name) {
        auto settings = this->configRepository->loadUserSettings("default");
        this->processingThread->reloadUserConfig(settings);
        delete settings;
    }

    this->logger->info("Configuration", "Deleted user settings " + name, {});
}

SystemSettings *Controller::ConfigurationController::getSystemSettings()
{
    return this->configRepository->loadSystemSettings();
}

void Controller::ConfigurationController::setInnerBrakeRange(Request *request)
{
    Assertion::jsonExistsAndString(request->getData(), "name");
    std::string name = request->getData()["name"];

    auto range = buildRange(request);

    auto oldConfig = this->configRepository->loadUserSettings(name);
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

    this->saveConfig(name, newConfig, oldConfig);
}

void Controller::ConfigurationController::setOuterBrakeRange(Request *request)
{
    Assertion::jsonExistsAndString(request->getData(), "name");
    std::string name = request->getData()["name"];
    
    Assertion::jsonExistsAndBool(request->getData(), "deactivated");
    auto range = buildRange(request);

    auto oldConfig = this->configRepository->loadUserSettings(name);
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

    this->saveConfig(name, newConfig, oldConfig);
}

void Controller::ConfigurationController::setRotationRadius(Request *request)
{
    Assertion::jsonExistsAndString(request->getData(), "name");
    std::string name = request->getData()["name"];
    
    Assertion::jsonExistsAndNumber(request->getData(), "rotationRadius");

    auto oldConfig = this->configRepository->loadUserSettings(name);
    auto newConfig = new UserSettings(oldConfig->getInnerBrakeRange()->clone(),
                                      oldConfig->getOuterBrakeRange()->clone(),
                                      request->getData()["rotationRadius"],
                                      oldConfig->getSoftStartSpeed(),
                                      oldConfig->getSoftStartAcceleration(),
                                      oldConfig->getAccelerationStages(),
                                      oldConfig->getAccelerationMode(),
                                      oldConfig->isAdaptiveAccelerationUIActivated(),
                                      oldConfig->isOuterBrakeDeactivated());

    this->saveConfig(name, newConfig, oldConfig);
}


void Controller::ConfigurationController::setSoftStart(Request *request)
{
    Assertion::jsonExistsAndString(request->getData(), "name");    
    Assertion::jsonExistsAndNumber(request->getData(), "speed");
    Assertion::jsonExistsAndNumber(request->getData(), "acceleration");

    std::string name = request->getData()["name"];
    
    auto oldConfig = this->configRepository->loadUserSettings(name);
    auto newConfig = new UserSettings(oldConfig->getInnerBrakeRange()->clone(),
                                      oldConfig->getOuterBrakeRange()->clone(),
                                      oldConfig->getRotationRadius(),
                                      request->getData()["speed"],
                                      request->getData()["acceleration"],
                                      oldConfig->getAccelerationStages(),
                                      oldConfig->getAccelerationMode(),
                                      oldConfig->isAdaptiveAccelerationUIActivated(),
                                      oldConfig->isOuterBrakeDeactivated());

    this->saveConfig(name, newConfig, oldConfig);
}

void Controller::ConfigurationController::setAccelerationStages(Request *request)
{
    Assertion::jsonExistsAndString(request->getData(), "name");
    Assertion::jsonExistsAndArray(request->getData(), "stages");
    Assertion::jsonExistsAndString(request->getData(), "mode");

    std::string name = request->getData()["name"];
    std::list<AccelerationStage> stages = {};

    for (auto& item : request->getData()["stages"]) {
        Assertion::jsonExistsAndNumber(item, "speed");
        Assertion::jsonExistsAndNumber(item, "acceleration");

        stages.push_back(AccelerationStage(item["speed"], item["acceleration"]));
    }

    auto oldConfig = this->configRepository->loadUserSettings(name);
    auto newConfig = new UserSettings(oldConfig->getInnerBrakeRange()->clone(),
                                      oldConfig->getOuterBrakeRange()->clone(),
                                      oldConfig->getRotationRadius(),
                                      oldConfig->getSoftStartSpeed(),
                                      oldConfig->getSoftStartAcceleration(),
                                      stages,
                                      UserSettings::stringToAccelerationMode(request->getData()["mode"]),
                                      oldConfig->isAdaptiveAccelerationUIActivated(),
                                      oldConfig->isOuterBrakeDeactivated());

    this->saveConfig(name, newConfig, oldConfig);
}

void Controller::ConfigurationController::setUserInterfaceSettings(Request* request)
{
    Assertion::jsonExistsAndString(request->getData(), "name");
    Assertion::jsonExistsAndBool(request->getData(), "activateAdaptiveAcceleration");

    std::string name = request->getData()["name"];
    
    auto oldConfig = this->configRepository->loadUserSettings(name);
    auto newConfig = new UserSettings(oldConfig->getInnerBrakeRange()->clone(),
                                      oldConfig->getOuterBrakeRange()->clone(),
                                      oldConfig->getRotationRadius(),
                                      oldConfig->getSoftStartSpeed(),
                                      oldConfig->getSoftStartAcceleration(),
                                      oldConfig->getAccelerationStages(),
                                      oldConfig->getAccelerationMode(),
                                      request->getData()["activateAdaptiveAcceleration"],
                                      oldConfig->isOuterBrakeDeactivated());

    this->saveConfig(name, newConfig, oldConfig);
}

Range* Controller::ConfigurationController::buildRange(Request *request)
{
    Assertion::jsonExistsAndNumber(request->getData(), "min");
    Assertion::jsonExistsAndNumber(request->getData(), "max");
    
    return new Range(request->getData()["min"], request->getData()["max"]);
}

void Controller::ConfigurationController::saveConfig(std::string name, UserSettings *newConfig, UserSettings *oldConfig)
{
    this->configRepository->saveUserSettings(name, newConfig);
    if (this->currentSettingsName == name) {
        this->processingThread->reloadUserConfig(newConfig);
    }

    delete oldConfig;
    delete newConfig;
}
