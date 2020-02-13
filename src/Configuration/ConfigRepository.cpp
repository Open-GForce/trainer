#include <utility>
#include <fstream>
#include <json.hpp>
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
    std::string content = this->loadFileContent();
    return ConfigRepository::decode(content);
}

void ConfigRepository::saveUserSettings(UserSettings *settings)
{
    std::ofstream configFile(basePath + "/user_settings.json");
    configFile << settings->toJSON().dump() << "\n";
    configFile.close();
}

std::string ConfigRepository::loadFileContent()
{
    std::string path = this->basePath + "/user_settings.json";
    std::ifstream ifs(path);

    if (!((bool) ifs)) {
        throw RuntimeException("Config file " + path + " does not exist");
    }

    std::string content;
    content.assign((std::istreambuf_iterator<char>(ifs)),
                   (std::istreambuf_iterator<char>()));

    return content;
}

UserSettings* ConfigRepository::decode(const std::string& fileContent)
{
    nlohmann::json data = nlohmann::json::parse(fileContent);

    return new UserSettings(
            ConfigRepository::parseRange(data, "innerBrakeRange"),
            ConfigRepository::parseRange(data, "outerBrakeRange"),
            parseRotationRadius(data)
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


