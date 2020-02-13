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
    public:
        const static std::string JSON_KEY_ROT_RADIUS;

    private:
        Range* innerBrakeRange;
        Range* outerBrakeRange;

        /**
         * Distance in meters between the center of the trainer and the passenger
         */
        double rotationRadius;

    public:
        UserSettings(Range *innerBrakeRange, Range *outerBrakeRange, double trainerRadius);

        ~UserSettings() override;

        nlohmann::json toJSON();
        Response *toResponse() override;

        [[nodiscard]] Range* getInnerBrakeRange() const;
        [[nodiscard]] Range* getOuterBrakeRange() const;
        [[nodiscard]] double getRotationRadius() const;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_USERSETTINGS_HPP
