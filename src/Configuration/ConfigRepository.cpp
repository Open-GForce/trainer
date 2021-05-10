#include <utility>
#include <fstream>
#include <nlohmann/json.hpp>
#include "../Utils/Exceptions/RuntimeException.hpp"
#include "ConfigRepository.hpp"

using namespace GForce::Configuration;
using namespace GForce::Utils::Exceptions;

ConfigRepository::ConfigRepository()
{
    this->basePath = "/etc/gforce_trainer";
}

ConfigRepository::ConfigRepository(std::string basePath) : basePath(std::move(basePath)) {}

UserSettings* ConfigRepository::loadUserSettings()
{
    std::string content = this->loadFileContent("user_settings.json");
    return ConfigRepository::decodeUserSettings(content);
}

SystemSettings *ConfigRepository::loadSystemSettings()
{
    std::string content = this->loadFileContent("system_settings.json");
    return ConfigRepository::decodeSystemSettings(content);
}

void ConfigRepository::saveUserSettings(UserSettings *settings)
{
    std::ofstream configFile(basePath + "/user_settings.json");
    configFile << settings->toJSON().dump() << "\n";
    configFile.close();
}

std::string ConfigRepository::loadFileContent(std::string file)
{
    std::string path = this->basePath + "/" + file;
    std::ifstream ifs(path);

    if (!((bool) ifs)) {
        throw RuntimeException("Config file " + path + " does not exist");
    }

    std::string content;
    content.assign((std::istreambuf_iterator<char>(ifs)),
                   (std::istreambuf_iterator<char>()));

    return content;
}

UserSettings* ConfigRepository::decodeUserSettings(const std::string &fileContent)
{
    nlohmann::json data = nlohmann::json::parse(fileContent);

    return new UserSettings(
            ConfigRepository::parseRange(data, "innerBrakeRange"),
            ConfigRepository::parseRange(data, "outerBrakeRange"),
            parseRotationRadius(data),
            parseSoftStartSpeed(data),
            parseSoftStartAcceleration(data),
            parseAccelerationStages(data),
            parseAccelerationMode(data),
            parseAdaptiveAcceleration(data),
            parseOuterBrakeDeactivated(data));
}

SystemSettings *ConfigRepository::decodeSystemSettings(const std::string &fileContent)
{
    nlohmann::json data = nlohmann::json::parse(fileContent);

    return new SystemSettings(
            parseBrakeProtocol(data),
            parseForceTable(data)
    );
}

Range* ConfigRepository::parseRange(nlohmann::json data, const std::string& key) {

    if (data.find(key) != data.end()
        && data[key].find("min") != data[key].end()
        && data[key].find("max") != data[key].end()
        && data[key]["min"].is_number()
        && data[key]["max"].is_number()) {
        return new Range(data[key]["min"], data[key]["max"]);
    }

    return new Range(0, 10000);
}

double ConfigRepository::parseRotationRadius(nlohmann::json data)
{
    if (data.find(UserSettings::JSON_KEY_ROT_RADIUS) != data.end() && data[UserSettings::JSON_KEY_ROT_RADIUS].is_number()) {
        return data[UserSettings::JSON_KEY_ROT_RADIUS];
    }

    return DEFAULT_ROTATION_RADIUS;
}

double ConfigRepository::parseSoftStartSpeed(nlohmann::json data)
{
    if (data.find(UserSettings::JSON_KEY_SOFT_START_SPEED) != data.end() && data[UserSettings::JSON_KEY_SOFT_START_SPEED].is_number()) {
        return data[UserSettings::JSON_KEY_SOFT_START_SPEED];
    }

    return DEFAULT_SOFT_START_SPEED;
}

int ConfigRepository::parseSoftStartAcceleration(nlohmann::json data)
{
    if (data.find(UserSettings::JSON_KEY_SOFT_START_ACC) != data.end() && data[UserSettings::JSON_KEY_SOFT_START_ACC].is_number()) {
        return data[UserSettings::JSON_KEY_SOFT_START_ACC];
    }

    return DEFAULT_SOFT_START_ACCELERATION;
}

std::list<AccelerationStage> ConfigRepository::parseAccelerationStages(nlohmann::json data)
{
    if (data.find(UserSettings::JSON_KEY_ACC_STAGES) == data.end()
        || !data.find(UserSettings::JSON_KEY_ACC_STAGES)->is_array()
        || data.find(UserSettings::JSON_KEY_ACC_STAGES)->empty()) {

        return {AccelerationStage(0, DEFAULT_SOFT_START_ACCELERATION)};
    }

    std::list<AccelerationStage> stages = {};

    for (auto& item : data[UserSettings::JSON_KEY_ACC_STAGES]) {
        if (item.find("speed") == item.end() || !item.find("speed")->is_number()
         || item.find("acceleration") == item.end() || !item.find("acceleration")->is_number()) {
            return {AccelerationStage(0, DEFAULT_SOFT_START_ACCELERATION)};
        }

        stages.push_back(AccelerationStage(item["speed"], item["acceleration"]));
    }

    return stages;
}

AccelerationMode ConfigRepository::parseAccelerationMode(nlohmann::json data)
{
    if (data.find(UserSettings::JSON_KEY_ACC_MODE) == data.end()
        || !data.find(UserSettings::JSON_KEY_ACC_MODE)->is_string()
        || data.find(UserSettings::JSON_KEY_ACC_MODE)->empty()) {

        return DEFAULT_ACC_MODE;
    }

    return UserSettings::stringToAccelerationMode(data[UserSettings::JSON_KEY_ACC_MODE]);
}

bool ConfigRepository::parseAdaptiveAcceleration(nlohmann::json data)
{
    if (data.find(UserSettings::JSON_KEY_ADAP_ACC_UI) != data.end() && data[UserSettings::JSON_KEY_ADAP_ACC_UI].is_boolean()) {
        return data[UserSettings::JSON_KEY_ADAP_ACC_UI];
    }

    return DEFAULT_ADAPTIVE_ACCELERATION_UI;
}

bool ConfigRepository::parseOuterBrakeDeactivated(nlohmann::json data)
{
    if (data.find(UserSettings::JSON_KEY_DEACTIVATE_OUTER_BRAKE) != data.end() && data[UserSettings::JSON_KEY_DEACTIVATE_OUTER_BRAKE].is_boolean()) {
        return data[UserSettings::JSON_KEY_DEACTIVATE_OUTER_BRAKE];
    }

    return DEFAULT_OUTER_BRAKE_DEACTIVATED;
}

BrakeSensorProtocol ConfigRepository::parseBrakeProtocol(nlohmann::json data)
{
    if (data.find(SystemSettings::JSON_KEY_BRAKE_PROTOCOL) == data.end()
        || !data.find(SystemSettings::JSON_KEY_BRAKE_PROTOCOL)->is_string()
        || data.find(SystemSettings::JSON_KEY_BRAKE_PROTOCOL)->empty()) {

        return BrakeSensorProtocol::IPNetwork;
    }

    return data[SystemSettings::JSON_KEY_BRAKE_PROTOCOL] == "CANopen"
        ? BrakeSensorProtocol::CANopen
        : BrakeSensorProtocol::IPNetwork;
}

std::map<int, int> ConfigRepository::parseForceTable(nlohmann::json data)
{
    if (data.find(SystemSettings::JSON_KEY_FORCE_TABLE) == data.end()
        || !data.find(SystemSettings::JSON_KEY_FORCE_TABLE)->is_array()
        || data.find(SystemSettings::JSON_KEY_FORCE_TABLE)->empty()) {

        return {
                {100, 0},
                {120, 1654},
                {140, 1744},
                {160, 1840},
                {180, 1936},
                {200, 2030},
                {250, 2252},
                {300, 2457},
                {350, 2648},
                {400, 2826},
                {450, 2995},
                {500, 3154},
                {550, 3307},
                {600, 3452},
                {650, 3592},
                {700, 3727}
        };
    }

    return data.at(SystemSettings::JSON_KEY_FORCE_TABLE).get<std::map<int, int>>();
}


