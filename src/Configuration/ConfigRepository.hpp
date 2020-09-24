#ifndef GFORCE_TRAINER_CONTROLLER_CONFIGREPOSITORY_HPP
#define GFORCE_TRAINER_CONTROLLER_CONFIGREPOSITORY_HPP

#include <string>
#include <json.hpp>
#include "UserSettings.hpp"

namespace GForce::Configuration {

class ConfigRepository
{
    protected:
        constexpr const static double DEFAULT_ROTATION_RADIUS     = 3.87;
        const static inline double DEFAULT_SOFT_START_SPEED       = 10;
        const static inline int DEFAULT_SOFT_START_ACCELERATION   = 1000;
        const static inline AccelerationMode DEFAULT_ACC_MODE     = AccelerationMode::targetSpeed;
        const static inline bool DEFAULT_ADAPTIVE_ACCELERATION_UI = false;

    private:
        std::string basePath;

        std::string loadFileContent();

        static UserSettings* decode(const std::string& fileContent);
        static Range* parseRange(nlohmann::json data, const std::string& key);
        static double parseRotationRadius(nlohmann::json data);
        static double parseSoftStartSpeed(nlohmann::json data);
        static int parseSoftStartAcceleration(nlohmann::json data);
        static std::list<AccelerationStage> parseAccelerationStages(nlohmann::json data);
        static AccelerationMode parseAccelerationMode(nlohmann::json data);
        static bool parseAdaptiveAcceleration(nlohmann::json data);

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
