#ifndef GFORCE_TRAINER_CONTROLLER_USERSETTINGS_HPP
#define GFORCE_TRAINER_CONTROLLER_USERSETTINGS_HPP

#include <json.hpp>
#include <list>
#include "../Processing/Range.hpp"
#include "../API/Websocket/ResponseCastInterface.hpp"
#include "../Processing/Acceleration/AccelerationStage.hpp"

using namespace GForce::Processing;
using namespace GForce::Processing::Acceleration;
using namespace GForce::API::Websocket;

namespace GForce::Configuration {

class UserSettings : public ResponseCastInterface
{
    public:
        const static inline std::string JSON_KEY_ROT_RADIUS       = "rotationRadius";
        const static inline std::string JSON_KEY_SOFT_START_ACC   = "softStartAcceleration";
        const static inline std::string JSON_KEY_SOFT_START_SPEED = "softStartSpeed";
        const static inline std::string JSON_KEY_ACC_STAGES       = "accelerationStages";
        const static inline std::string JSON_KEY_ADAP_ACC_UI      = "useAdaptiveAccelerationUserInterface";

    private:
        Range* innerBrakeRange;
        Range* outerBrakeRange;

        /**
         * Distance in meters between the center of the trainer and the passenger
         */
        double rotationRadius;

        /**
        * Acceleration configuration dependent on target speed
        */
        std::list<AccelerationStage> accelerationStages;

        /**
         * Speed in 1/min, up to which the static soft start acceleration takes effect
         */
        double softStartSpeed;

        /**
         * Raw soft start acceleration
         */
        int softStartAcceleration;

        /**
         * Activate simplified acceleration button during operation
         */
        bool useAdaptiveAccelerationUserInterface;

    public:
        UserSettings(Range *innerBrakeRange, Range *outerBrakeRange, double trainerRadius,
                     double softStartSpeed, int softStartAcceleration, std::list<AccelerationStage> stages,
                     bool useAdaptiveAccelerationButtons);

        ~UserSettings() override;

        nlohmann::json toJSON();
        Response *toResponse() override;

        [[nodiscard]] Range* getInnerBrakeRange() const;
        [[nodiscard]] Range* getOuterBrakeRange() const;
        [[nodiscard]] double getRotationRadius() const;
        [[nodiscard]] const std::list<AccelerationStage> getAccelerationStages() const;
        [[nodiscard]] double getSoftStartSpeed() const;
        [[nodiscard]] int getSoftStartAcceleration() const;
        [[nodiscard]] bool isAdaptiveAccelerationUIActivated() const;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_USERSETTINGS_HPP
