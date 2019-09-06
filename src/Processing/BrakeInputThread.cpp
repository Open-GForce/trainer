#include <string>
#include "BrakeInputThread.hpp"
#include <exception>

using namespace GForce::Processing;

BrakeInputThread::BrakeInputThread(ADCSensorInterface *sensor, LoggerInterface *logger) : sensor(sensor), logger(logger)
{
    this->firstBrake = 0;
    this->secondBrake = 0;
    this->stopped = false;
}

void BrakeInputThread::start()
{
    while (!stopped) {
        try {
            this->firstBrake = scaleSignedInt(this->sensor->read(0));
            this->secondBrake = scaleSignedInt(this->sensor->read(3));
        } catch (std::exception &e) {
            this->firstBrake = 0;
            this->secondBrake = 0;

            this->logger->error("BrakeInputThread => " + std::string(e.what()));
        }
    }
}

int BrakeInputThread::scaleSignedInt(int value)
{
    return value > 32768 ? (0 - (65536 - value)) : value;
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
