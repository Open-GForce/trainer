#ifndef GFORCE_TRAINER_CONTROLLER_CONFIGREPOSITORY_HPP
#define GFORCE_TRAINER_CONTROLLER_CONFIGREPOSITORY_HPP

#include <string>
#include <nlohmann/json.hpp>
#include "UserSettings.hpp"
#include "SystemSettings.hpp"

namespace GForce::Configuration {

class ConfigRepository
{
    protected:
        const static inline std::string USER_SETTINGS_PREFIX = "user_settings.";
        const static inline std::string SETTINGS_SUFFIX      = ".json";

        constexpr const static double DEFAULT_ROTATION_RADIUS     = 3.87;
        const static inline double DEFAULT_SOFT_START_SPEED       = 10;
        const static inline int DEFAULT_SOFT_START_ACCELERATION   = 1000;
        const static inline AccelerationMode DEFAULT_ACC_MODE     = AccelerationMode::targetSpeed;
        const static inline bool DEFAULT_ADAPTIVE_ACCELERATION_UI = false;
        const static inline bool DEFAULT_OUTER_BRAKE_DEACTIVATED  = false;

    private:
        std::string basePath;

        /**
         * Loads the given configuration file.
         * Throws an exception if file is not existing or non readable
         */
        std::string loadFileContent(std::string file);

        /**
         * Decodes the content of user_settings.json to DTO
         */
        static UserSettings* decodeUserSettings(const std::string& fileContent);

        /**
         * Decodes the content of system_settings.json to DTO
         */
        static SystemSettings* decodeSystemSettings(const std::string& fileContent);

        /**
         * Parser for parameters of user settings
         */
        static Range* parseRange(nlohmann::json data, const std::string& key);
        static double parseRotationRadius(nlohmann::json data);
        static double parseSoftStartSpeed(nlohmann::json data);
        static int parseSoftStartAcceleration(nlohmann::json data);
        static std::list<AccelerationStage> parseAccelerationStages(nlohmann::json data);
        static AccelerationMode parseAccelerationMode(nlohmann::json data);
        static bool parseAdaptiveAcceleration(nlohmann::json data);
        static bool parseOuterBrakeDeactivated(nlohmann::json data);

        /**
         * Parser for parameters of system settings
         */
        static BrakeSensorProtocol parseBrakeProtocol(nlohmann::json data);
        static std::map<int, int> parseForceTable(nlohmann::json data);

        /**
         * Encodes/Decodes base64 names
         */
        static std::string decodeName(std::string data);
        static std::string encodeName(std::string data);

    public:
        ConfigRepository();
        explicit ConfigRepository(std::string basePath);

        /**
         * Loads the user settings based on name.
         * Ownership of returned object moves to caller
         */
        virtual UserSettings* loadUserSettings(std::string name);

        /**
         * Deletes the given user settings.
         * Deletion of default settings throws an exception
         */
        virtual void deleteUserSettings(std::string name);

        /**
         * Returns a name list of saved user settings
         */
        virtual std::list<std::string> getAvailableUserSettings();

        /**
         * Ownership of returned object moves to caller
         */
        virtual SystemSettings* loadSystemSettings();

        /**
         * Saves the user settings based on name.
        * Ownership of object stays at caller
        */
        virtual void saveUserSettings(std::string name, UserSettings* settings);
};

}

#endif //GFORCE_TRAINER_CONTROLLER_CONFIGREPOSITORY_HPP
