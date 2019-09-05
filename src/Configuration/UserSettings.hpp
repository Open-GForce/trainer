#ifndef GFORCE_TRAINER_CONTROLLER_USERSETTINGS_HPP
#define GFORCE_TRAINER_CONTROLLER_USERSETTINGS_HPP

#include <json.hpp>
#include "../Processing/Range.hpp"
#include "../API/Websocket/ResponseCastInterface.hpp"

using namespace GForce::Processing;
using namespace GForce::API::Websocket;

namespace GForce::Configuration {

class UserSettings : public ResponseCastInterface
{
    private:
        Range* innerBrakeRange;
        Range* outerBrakeRange;

    public:
        UserSettings(Range *innerBrakeRange, Range *outerBrakeRange);
        ~UserSettings() override;

        nlohmann::json toJSON();
        Response *toResponse() override;

        Range* getInnerBrakeRange() const;
        Range* getOuterBrakeRange() const;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_USERSETTINGS_HPP
