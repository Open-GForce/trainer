#ifndef GFORCE_TRAINER_CONTROLLER_ACCELERATIONSTAGE_HPP
#define GFORCE_TRAINER_CONTROLLER_ACCELERATIONSTAGE_HPP

namespace GForce::Processing::Acceleration {

class AccelerationStage
{
    private:
        /**
         * Speed in 1/min, at which this stage gets active
         */
        double speed;

        /**
         * Raw acceleration ramp, sent to MoviDrive, as soon stage is active
         */
        int acceleration;

    public:
        AccelerationStage(double speed, int acceleration);

        bool operator<(const AccelerationStage &a) const;

        double getSpeed() const;
        int getAcceleration() const;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_ACCELERATIONSTAGE_HPP
