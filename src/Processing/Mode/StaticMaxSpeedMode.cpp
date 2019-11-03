#include "StaticMaxSpeedMode.hpp"

using namespace GForce::Processing::Mode;

std::string StaticMaxSpeedMode::getIdentifier()
{
    return StaticMaxSpeedMode::IDENTIFIER;
}

double StaticMaxSpeedMode::getTargetSpeed(double innerBrake, double outerBrake)
{
    return 1;
}
