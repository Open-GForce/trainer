#include "UserSettings.hpp"

using namespace GForce::Configuration;

UserSettings::UserSettings(Range *innerBrakeRange, Range *outerBrakeRange) : innerBrakeRange(innerBrakeRange), outerBrakeRange(outerBrakeRange) {}

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
    };

    return data;
}

Range *UserSettings::getInnerBrakeRange() const {
    return innerBrakeRange;
}

Range *UserSettings::getOuterBrakeRange() const {
    return outerBrakeRange;
}


