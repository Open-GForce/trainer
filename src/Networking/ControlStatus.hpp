#ifndef GFORCE_TRAINER_CONTROLLER_CONTROLSTATUS_HPP
#define GFORCE_TRAINER_CONTROLLER_CONTROLSTATUS_HPP

#include <cstdint>
#include <nlohmann/json.hpp>

namespace GForce::Networking {

class ControlStatus
{
    private:
        /**
         * Stops the engine "softly" with the default deceleration
         */
        bool softBrakeActivated;

        /**
         * Stops the engine "hard" with the configured deceleration (P136 or P146)
         */
        bool hardBrakeActivated;

        /**
         * Activates the mechanical brake (if existing) + power resistor
         * Brake keeps activated after engine has been stopped
         */
        bool holdingBrakeActivated;

        /**
         * Stops the engine with a "more complex" configured algorithm (e.g exact position)
         */
        bool holdControlActivated;

    public:
        ControlStatus(bool softBrakeActivated, bool hardBrakeActivated, bool holdingBrakeActivated, bool holdControlActivated);

        /**
         * Converts the flags to the MoviDrive control word
         * s. 7.4.3 of https://download.sew-eurodrive.com/download/pdf/11264918.pdf
         */
        uint16_t toBinary();

        static ControlStatus* release();
        static ControlStatus* softBrake();
        static ControlStatus* hardBrake();
        static ControlStatus* holdingBrake();
        static ControlStatus* holdingControl();

        nlohmann::json toJson();

        bool isSoftBrakeActivated() const;

};

}

#endif //GFORCE_TRAINER_CONTROLLER_CONTROLSTATUS_HPP
