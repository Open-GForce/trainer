#include "UserSettings.hpp"

using namespace GForce::Configuration;

UserSettings::UserSettings(Range *innerBrakeRange, Range *outerBrakeRange) : innerBrakeRange(innerBrakeRange), outerBrakeRange(outerBrakeRange) {}

UserSettings::~UserSettings()
{
    delete this->innerBrakeRange;
    delete this->outerBrakeRange;
}

Range *UserSettings::getInnerBrakeRange() const {
    return innerBrakeRange;
}

Range *UserSettings::getOuterBrakeRange() const {
    return outerBrakeRange;
}


