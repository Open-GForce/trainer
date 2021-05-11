#ifndef GFORCE_TRAINER_CONTROLLER_USERSETTINGS_HPP
#define GFORCE_TRAINER_CONTROLLER_USERSETTINGS_HPP

#include <nlohmann/json.hpp>
#include <list>
#include "../Processing/Range.hpp"
#include "../API/Websocket/ResponseCastInterface.hpp"
#include "../Processing/Acceleration/AccelerationStage.hpp"

using namespace GForce::Processing;
using namespace GForce::Processing::Acceleration;
using namespace GForce::API::Websocket;

namespace GForce::Configuration {

enum AccelerationMode {
    // Determines the acceleration based on difference between current and target speed
    differential,
    // Determines the acceleration based on target speed
    targetSpeed,
    // Determines the acceleration mode on current speed
    currentSpeed
};

class UserSettings : public ResponseCastInterface
{
    public:
        const static inline std::string JSON_KEY_SOFT_START_ACC         = "softStartAcceleration";
        const static inline std::string JSON_KEY_SOFT_START_SPEED       = "softStartSpeed";
        const static inline std::string JSON_KEY_ACC_STAGES             = "accelerationStages";
        const static inline std::string JSON_KEY_ACC_MODE               = "accelerationMode";
        const static inline std::string JSON_KEY_ADAP_ACC_UI            = "useAdaptiveAccelerationUserInterface";
        const static inline std::string JSON_KEY_DEACTIVATE_OUTER_BRAKE = "outerBrakeDeactivated";

    private:
        Range* innerBrakeRange;
        Range* outerBrakeRange;

        /**
        * Acceleration configuration dependent on target speed
        */
        std::list<AccelerationStage> accelerationStages;

        /**
         * Algorithm for selecting the acceleration stage
         */
        AccelerationMode accelerationMode;

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

        /**
         * Ignore input of outer brake
         */
        bool outerBrakeDeactivated;

    public:
        UserSettings(Range *innerBrakeRange, Range *outerBrakeRange, double softStartSpeed, int softStartAcceleration,
                     std::list<AccelerationStage> stages, AccelerationMode accelerationMode,
                     bool useAdaptiveAccelerationButtons, bool deactivateOuterBrake);

        ~UserSettings() override;

        nlohmann::json toJSON();
        Response *toResponse() override;

        [[nodiscard]] Range* getInnerBrakeRange() const;
        [[nodiscard]] Range* getOuterBrakeRange() const;
        [[nodiscard]] const std::list<AccelerationStage> getAccelerationStages() const;
        [[nodiscard]] AccelerationMode getAccelerationMode() const;
        [[nodiscard]] double getSoftStartSpeed() const;
        [[nodiscard]] int getSoftStartAcceleration() const;
        [[nodiscard]] bool isAdaptiveAccelerationUIActivated() const;
        [[nodiscard]] bool isOuterBrakeDeactivated() const;

        static AccelerationMode stringToAccelerationMode(std::string input) {
            if (input == "differential") {
                return AccelerationMode::differential;
            }

            if (input == "currentSpeed") {
                return AccelerationMode::currentSpeed;
            }

            return AccelerationMode::targetSpeed;
        }
};
}

#endif //GFORCE_TRAINER_CONTROLLER_USERSETTINGS_HPP
