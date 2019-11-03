#ifndef GFORCE_TRAINER_CONTROLLER_OPERATIONMODE_HPP
#define GFORCE_TRAINER_CONTROLLER_OPERATIONMODE_HPP

namespace GForce::Processing::Mode {

class OperationMode
{
    public:
        /**
         * Returns a unique identifier which describes the inheritance
         */
        virtual std::string getIdentifier() = 0;

        /**
         * Calculates the target speed bases on brake inputs
         *
         * @param innerBrake Percentage value (0-1) of inner brake input
         * @param outerBrake Percentage value (0-1) of outer brake input
         *
         * @return Returns the percentage (0-1) of the max. rotation speed
         */
        virtual double getTargetSpeed(double innerBrake, double outerBrake) = 0;

        OperationMode() = default;
        virtual ~OperationMode() = default;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_OPERATIONMODE_HPP
