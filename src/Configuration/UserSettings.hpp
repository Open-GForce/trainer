#ifndef GFORCE_TRAINER_CONTROLLER_USERSETTINGS_HPP
#define GFORCE_TRAINER_CONTROLLER_USERSETTINGS_HPP

#include <json.hpp>
#include "../Processing/Range.hpp"

using namespace GForce::Processing;

namespace GForce::Configuration {

class UserSettings
{
    private:
        Range* innerBrakeRange;
        Range* outerBrakeRange;

    public:
        UserSettings(Range *innerBrakeRange, Range *outerBrakeRange);
        virtual ~UserSettings();

        nlohmann::json toJSON();

        Range* getInnerBrakeRange() const;
        Range* getOuterBrakeRange() const;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_USERSETTINGS_HPP
