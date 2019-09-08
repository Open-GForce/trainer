#include <string>
#include "BrakeInputReceiveThread.hpp"
#include <exception>

using namespace GForce::Processing::BrakeInput;

BrakeInputReceiveThread::BrakeInputReceiveThread(ADCSensorInterface *sensor, LoggerInterface *logger) : sensor(sensor), logger(logger)
{
    this->firstBrake = 0;
    this->secondBrake = 0;
    this->stopped = false;
}

void BrakeInputReceiveThread::start()
{
    while (!stopped) {
        try {
            this->firstBrake = scaleSignedInt(this->sensor->read(1));
            this->secondBrake = scaleSignedInt(this->sensor->read(3));
        } catch (std::exception &e) {
            this->firstBrake = 0;
            this->secondBrake = 0;

            this->logger->error("BrakeInputThread => " + std::string(e.what()));
        }
    }
}

int BrakeInputReceiveThread::scaleSignedInt(int value)
{
    return value > 32768 ? (0 - (65536 - value)) : value;
}

void BrakeInputReceiveThread::stop() {
    this->stopped = true;
}

int BrakeInputReceiveThread::getFirstBrake() const {
    return firstBrake;
}

int BrakeInputReceiveThread::getSecondBrake() const {
    return secondBrake;
}
