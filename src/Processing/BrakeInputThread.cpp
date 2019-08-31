#include <bits/exception.h>
#include <string>
#include "BrakeInputThread.hpp"

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
            this->firstBrake = this->sensor->read(0);
            this->secondBrake = this->sensor->read(1);
        } catch (std::exception &e) {
            this->firstBrake = 0;
            this->secondBrake = 0;

            this->logger->error("BrakeInputThread => " + std::string(e.what()));
        }
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