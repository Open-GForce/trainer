#ifndef GFORCE_TRAINER_CONTROLLER_SYSTEMSETTINGS_HPP
#define GFORCE_TRAINER_CONTROLLER_SYSTEMSETTINGS_HPP

#include <nlohmann/json.hpp>
#include "../API/Websocket/ResponseCastInterface.hpp"

using namespace GForce::API::Websocket;

namespace GForce::Configuration {

enum BrakeSensorProtocol {IPNetwork, CANopen};

class SystemSettings : public ResponseCastInterface
{
    public:
        const static inline std::string JSON_KEY_BRAKE_PROTOCOL = "brakeSensorProtocol";
        const static inline std::string JSON_KEY_FORCE_TABLE    = "forceTable";

    private:
        /**
         * Type of installed brake sensor
         */
        BrakeSensorProtocol brakeSensorProtocol;

        /**
         * Fixed force (G) <-> speed (u/min) lookup table
         * Bot values are scaled by factor 100
         */
        std::map<int, int> forceTable;

    public:
        SystemSettings(GForce::Configuration::BrakeSensorProtocol brakeSensorProtocol,
                       std::map<int, int> forceTable);

        nlohmann::json toJSON();
        Response *toResponse() override;

        BrakeSensorProtocol getBrakeSensorProtocol() const;

        const std::map<int, int> &getForceTable() const;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_SYSTEMSETTINGS_HPP
