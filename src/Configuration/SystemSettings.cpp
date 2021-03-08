#include "SystemSettings.hpp"

using namespace GForce::Configuration;

SystemSettings::SystemSettings(GForce::Configuration::BrakeSensorProtocol brakeSensorProtocol)
        : brakeSensorProtocol(brakeSensorProtocol) {}

nlohmann::json SystemSettings::toJSON()
{
    std::string protocol;

    switch (this->brakeSensorProtocol) {
        case CANopen: protocol = "CANopen"; break;
        default: protocol = "IPNetwork";
    }

    return {
        {JSON_KEY_BRAKE_PROTOCOL, protocol}
    };
}

BrakeSensorProtocol SystemSettings::getBrakeSensorProtocol() const {
    return brakeSensorProtocol;
}
