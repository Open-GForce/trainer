#ifndef GFORCE_TRAINER_CONTROLLER_REGULARSPIRALMODE_HPP
#define GFORCE_TRAINER_CONTROLLER_REGULARSPIRALMODE_HPP

#include "OperationMode.hpp"

namespace GForce::Processing::Mode {

class RegularSpiralMode : public OperationMode
{
    public:
        double getTargetSpeed(double innerBrake, double outerBrake) override;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_REGULARSPIRALMODE_HPP
