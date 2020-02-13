#include "UserSettings.hpp"

using namespace GForce::Configuration;

const std::string UserSettings::JSON_KEY_ROT_RADIUS = "rotationRadius";

UserSettings::UserSettings(Range *innerBrakeRange, Range *outerBrakeRange, double trainerRadius) : innerBrakeRange(
        innerBrakeRange), outerBrakeRange(outerBrakeRange), rotationRadius(trainerRadius) {}

UserSettings::~UserSettings()
{
    delete this->innerBrakeRange;
    delete this->outerBrakeRange;
}

Response *UserSettings::toResponse()
{
    return new Response("userSettings", this->toJSON());
}

nlohmann::json UserSettings::toJSON()
{
    nlohmann::json data = {
            {"innerBrakeRange", {
                {"min", this->innerBrakeRange->getMin()},
                {"max", this->innerBrakeRange->getMax()},
            }},
            {"outerBrakeRange", {
                {"min", this->outerBrakeRange->getMin()},
                {"max", this->outerBrakeRange->getMax()},
            }},
            {JSON_KEY_ROT_RADIUS, this->rotationRadius}
    };

    return data;
}

Range *UserSettings::getInnerBrakeRange() const {
    return innerBrakeRange;
}

Range *UserSettings::getOuterBrakeRange() const {
    return outerBrakeRange;
}

double UserSettings::getRotationRadius() const {
    return rotationRadius;
}


