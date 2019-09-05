#ifndef GFORCE_TRAINER_CONTROLLER_CONFIGREPOSITORY_HPP
#define GFORCE_TRAINER_CONTROLLER_CONFIGREPOSITORY_HPP

#include <string>
#include <json.hpp>
#include "UserSettings.hpp"

namespace GForce::Configuration {

class ConfigRepository
{
    private:
        std::string basePath;

        std::string loadFileContent();

        static UserSettings* decode(const std::string& fileContent);
        static Range* parseRange(nlohmann::json data, const std::string& key);

    public:
        ConfigRepository();
        explicit ConfigRepository(std::string basePath);

        /**
         * Ownership of returned object moves to caller
         */
        virtual UserSettings* loadUserSettings();

        /**
        * Ownership of object stays at caller
        */
        virtual void saveUserSettings(UserSettings* settings);
};

}

#endif //GFORCE_TRAINER_CONTROLLER_CONFIGREPOSITORY_HPP
