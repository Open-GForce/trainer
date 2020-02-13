#include "ConfigurationController.hpp"
#include "../../Utils/Assertions/Assertion.hpp"

using namespace GForce::Utils::Assertions;
using namespace GForce::API::Controller;

ConfigurationController::ConfigurationController(ProcessingThread *processingThread,ConfigRepository *configRepository) :
    processingThread(processingThread), configRepository(configRepository) {}

UserSettings* ConfigurationController::getUserSettings()
{
    return this->configRepository->loadUserSettings();
}

void ConfigurationController::setInnerBrakeRange(Request *request)
{
    auto range = buildRange(request);

    auto oldConfig = this->configRepository->loadUserSettings();
    auto newConfig = new UserSettings(range, oldConfig->getOuterBrakeRange()->clone(), oldConfig->getRotationRadius());

    this->saveConfig(newConfig, oldConfig);
}

void ConfigurationController::setOuterBrakeRange(Request *request)
{
    auto range = buildRange(request);

    auto oldConfig = this->configRepository->loadUserSettings();
    auto newConfig = new UserSettings(oldConfig->getInnerBrakeRange()->clone(), range, oldConfig->getRotationRadius());

    this->saveConfig(newConfig, oldConfig);
}

Range *ConfigurationController::buildRange(Request *request)
{
    Assertion::jsonExistsAndNumber(request->getData(), "min");
    Assertion::jsonExistsAndNumber(request->getData(), "max");

    return new Range(request->getData()["min"], request->getData()["max"]);
}

void ConfigurationController::saveConfig(UserSettings *newConfig, UserSettings *oldConfig)
{
    this->configRepository->saveUserSettings(newConfig);
    this->processingThread->reloadUserConfig(newConfig);

    delete oldConfig;
    delete newConfig;
}
