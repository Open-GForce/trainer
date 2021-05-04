#include "SystemSettings.hpp"

using namespace GForce::Configuration;

SystemSettings::SystemSettings(GForce::Configuration::BrakeSensorProtocol brakeSensorProtocol,
                               std::map<int, int> forceTable)
        : brakeSensorProtocol(brakeSensorProtocol), forceTable(forceTable) {}

nlohmann::json SystemSettings::toJSON()
{
    std::string protocol;

    switch (this->brakeSensorProtocol) {
        case CANopen: protocol = "CANopen"; break;
        default: protocol = "IPNetwork";
    }

    return {
        {JSON_KEY_BRAKE_PROTOCOL, protocol},
        {JSON_KEY_FORCE_TABLE, this->forceTable},
    };
}

Response *SystemSettings::toResponse()
{
    return new Response("systemSettings", this->toJSON());
}

BrakeSensorProtocol SystemSettings::getBrakeSensorProtocol() const {
    return brakeSensorProtocol;
}

const std::map<int, int> &SystemSettings::getForceTable() const {
    return forceTable;
}
