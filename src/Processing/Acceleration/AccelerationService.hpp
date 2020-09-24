#ifndef GFORCE_TRAINER_CONTROLLER_ACCELERATIONSERVICE_HPP
#define GFORCE_TRAINER_CONTROLLER_ACCELERATIONSERVICE_HPP

#include <list>
#include "AccelerationStage.hpp"
#include "../../Configuration/UserSettings.hpp"

using namespace GForce::Configuration;

namespace GForce::Processing::Acceleration {

class AccelerationService
{
    private:
        /**
         * Acceleration configuration dependent on target speed
         */
        std::list<AccelerationStage> stages;

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

        virtual void setStages(const std::list<AccelerationStage> &newStages);
        virtual void setSoftStartSpeed(double speed);
        virtual void setSoftStartAcceleration(int value);
        virtual void setAccelerationMode(AccelerationMode mode);
};

}

#endif //GFORCE_TRAINER_CONTROLLER_ACCELERATIONSERVICE_HPP
