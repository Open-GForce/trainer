#ifndef GFORCE_TRAINER_CONTROLLER_STATICMAXSPEEDMODE_HPP
#define GFORCE_TRAINER_CONTROLLER_STATICMAXSPEEDMODE_HPP

#include <string>
#include "OperationMode.hpp"

namespace GForce::Processing::Mode {

class StaticMaxSpeedMode : public OperationMode
{
    public:
        inline static const std::string IDENTIFIER = "staticMaxSpeed";

    public:
        std::string getIdentifier() override;

        double getTargetSpeed(double innerBrake, double outerBrake) override;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_STATICMAXSPEEDMODE_HPP
