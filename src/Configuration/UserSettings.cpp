#include "UserSettings.hpp"

using namespace GForce::Configuration;

UserSettings::UserSettings(Range *innerBrakeRange, Range *outerBrakeRange, double softStartSpeed,
                           int softStartAcceleration,
                           std::list<AccelerationStage> stages, AccelerationMode accelerationMode,
                           bool useAdaptiveAccelerationButtons, bool deactivateOuterBrake)
        : innerBrakeRange(innerBrakeRange), outerBrakeRange(outerBrakeRange),
          softStartSpeed(softStartSpeed),
          softStartAcceleration(softStartAcceleration),
          accelerationStages(stages), useAdaptiveAccelerationUserInterface(useAdaptiveAccelerationButtons),
          accelerationMode(accelerationMode), outerBrakeDeactivated(deactivateOuterBrake) {}

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
            {JSON_KEY_SOFT_START_SPEED, this->softStartSpeed},
            {JSON_KEY_SOFT_START_ACC, this->softStartAcceleration},
            {JSON_KEY_ACC_STAGES, {}},
            {JSON_KEY_ACC_MODE, "unknown"},
            {JSON_KEY_ADAP_ACC_UI, this->useAdaptiveAccelerationUserInterface},
            {JSON_KEY_DEACTIVATE_OUTER_BRAKE, this->isOuterBrakeDeactivated()},
    };

    switch (this->accelerationMode) {
        case differential:
            data[JSON_KEY_ACC_MODE] = "differential"; break;
        case targetSpeed:
            data[JSON_KEY_ACC_MODE] = "targetSpeed"; break;
        case currentSpeed:
            data[JSON_KEY_ACC_MODE] = "currentSpeed"; break;
    }

    for (auto const& stage : this->accelerationStages) {
        data[JSON_KEY_ACC_STAGES].push_back({
             {"speed", stage.getSpeed()},
             {"acceleration", stage.getAcceleration()},
        });
    }

    return data;
}

Range *UserSettings::getInnerBrakeRange() const {
    return innerBrakeRange;
}

Range *UserSettings::getOuterBrakeRange() const {
    return outerBrakeRange;
}

const std::list<AccelerationStage> UserSettings::getAccelerationStages() const {
    return accelerationStages;
}

double UserSettings::getSoftStartSpeed() const {
    return softStartSpeed;
}

int UserSettings::getSoftStartAcceleration() const {
    return softStartAcceleration;
}

bool UserSettings::isAdaptiveAccelerationUIActivated() const {
    return useAdaptiveAccelerationUserInterface;
}

AccelerationMode UserSettings::getAccelerationMode() const {
    return accelerationMode;
}

bool UserSettings::isOuterBrakeDeactivated() const {
    return outerBrakeDeactivated;
}


