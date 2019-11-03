#include "RegularSpiralMode.hpp"

using namespace GForce::Processing::Mode;

std::string RegularSpiralMode::getIdentifier()
{
    return RegularSpiralMode::IDENTIFIER;
}

double RegularSpiralMode::getTargetSpeed(double innerBrake, double outerBrake)
{
    double speed = innerBrake - outerBrake;
    return speed < 0 ? 0 : speed;
}
