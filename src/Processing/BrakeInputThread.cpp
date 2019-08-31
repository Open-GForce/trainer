#include "BrakeInputThread.hpp"

using namespace GForce::Processing;

BrakeInputThread::BrakeInputThread(ADCSensorInterface *sensor) : sensor(sensor)
{
    this->firstBrake = 0;
    this->secondBrake = 0;
    this->stopped = false;
}

void BrakeInputThread::start()
{
    while (!stopped) {
        this->firstBrake = this->sensor->read(0);
        this->secondBrake = this->sensor->read(1);
    }
}

void BrakeInputThread::stop() {
    this->stopped = true;
}

int BrakeInputThread::getFirstBrake() const {
    return firstBrake;
}

int BrakeInputThread::getSecondBrake() const {
    return secondBrake;
}
