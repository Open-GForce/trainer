#include "AccelerationStage.hpp"

using namespace GForce::Processing::Acceleration;

AccelerationStage::AccelerationStage(double speed, int acceleration) : speed(speed), acceleration(acceleration) {}

bool AccelerationStage::operator<(const AccelerationStage &a) const {
    return this->speed < a.getSpeed();
}

double AccelerationStage::getSpeed() const {
    return speed;
}

int AccelerationStage::getAcceleration() const {
    return acceleration;
}

