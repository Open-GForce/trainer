#ifndef GFORCE_TRAINER_CONTROLLER_SYSTEMSETTINGS_HPP
#define GFORCE_TRAINER_CONTROLLER_SYSTEMSETTINGS_HPP

#include <nlohmann/json.hpp>

namespace GForce::Configuration {

enum BrakeSensorProtocol {IPNetwork, CANopen};

class SystemSettings
{
    public:
        const static inline std::string JSON_KEY_BRAKE_PROTOCOL = "brakeSensorProtocol";

    private:
        /**
         * Type of installed brake sensor
         */
        BrakeSensorProtocol brakeSensorProtocol;

    public:
        SystemSettings(BrakeSensorProtocol brakeSensorProtocol);

        nlohmann::json toJSON();

        BrakeSensorProtocol getBrakeSensorProtocol() const;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_SYSTEMSETTINGS_HPP
