#ifndef GFORCE_TRAINER_CONTROLLER_REGULARSPIRALMODE_HPP
#define GFORCE_TRAINER_CONTROLLER_REGULARSPIRALMODE_HPP

#include <string>
#include "OperationMode.hpp"

namespace GForce::Processing::Mode {

class RegularSpiralMode : public OperationMode
{
    public:
        inline static const std::string IDENTIFIER = "regularSpiral";

    public:
        std::string getIdentifier() override;

        double getTargetSpeed(double innerBrake, double outerBrake) override;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_REGULARSPIRALMODE_HPP
