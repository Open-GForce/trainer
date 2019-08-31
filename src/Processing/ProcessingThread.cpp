#include <thread>
#include "ProcessingThread.hpp"

using namespace GForce::Processing;

ProcessingThread::ProcessingThread(ProcessingService *service) : service(service)
{
    this->brakeInputThread = nullptr;
    this->cycleInterval = 10;
    this->stopped = false;
}

void ProcessingThread::start(BrakeInputThread *brakeThread)
{
    this->brakeInputThread = brakeThread;

    std::chrono::milliseconds period(this->cycleInterval);
    std::chrono::time_point next;

    while (!stopped) {
        next = std::chrono::high_resolution_clock::now() + period;
        this->loop();
        std::this_thread::sleep_until(next);
    }
}

void ProcessingThread::loop()
{

}

void ProcessingThread::stop() {
    this->stopped = true;
}

void ProcessingThread::setCycleInterval(int value) {
    this->cycleInterval = value;
}


