#ifndef GFORCE_TRAINER_CONTROLLER_ACCELERATIONSERVICE_HPP
#define GFORCE_TRAINER_CONTROLLER_ACCELERATIONSERVICE_HPP

#include <list>
#include "AccelerationStage.hpp"

namespace GForce::Processing::Acceleration {

class AccelerationService
{
    private:
        /**
         * Acceleration configuration dependent on target speed
         */
        std::list<AccelerationStage> stages;

        /**
         * Speed in 1/min, up to which the static soft start acceleration takes effect
         */
        double softStartSpeed;

        /**
         * Raw soft start acceleration
         */
        int softStartAcceleration;

    public:
        AccelerationService();

        /**
         * Calculates the acceleration in relation to current and target speed
         *
         * @param currentSpeed Current speed in 1/min
         * @param targetSpeed  Target speed in 1/min
         * @return Raw acceleration
         */
        virtual int getAcceleration(double currentSpeed, double targetSpeed);

        void setStages(const std::list<AccelerationStage> &newStages);
        void setSoftStartSpeed(double speed);
        void setSoftStartAcceleration(int value);
};

}

#endif //GFORCE_TRAINER_CONTROLLER_ACCELERATIONSERVICE_HPP
