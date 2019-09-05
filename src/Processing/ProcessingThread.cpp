#include <thread>
#include "ProcessingThread.hpp"

using namespace GForce::Processing;

ProcessingThread::ProcessingThread(ProcessingService *service) : service(service)
{
    this->brakeInputThread = nullptr;
    this->websocketThread = nullptr;
    this->cycleInterval = 10;
    this->stopped = false;
}

void ProcessingThread::start(BrakeInputThread* brakeThread, Websocket::ServerThread* serverThread)
{
    this->brakeInputThread = brakeThread;
    this->websocketThread = serverThread;

    std::chrono::milliseconds period(this->cycleInterval);
    std::chrono::high_resolution_clock::time_point next;

    while (!stopped) {
        next = std::chrono::high_resolution_clock::now() + period;
        this->loop();
        std::this_thread::sleep_until(next);
    }
}

void ProcessingThread::loop()
{
    this->loopMutex.lock();
    int firstInput = this->brakeInputThread->getFirstBrake();
    int secondInput = this->brakeInputThread->getSecondBrake();

    this->service->setFirstBrakeInput(firstInput);
    this->service->setSecondBrakeInput(secondInput);
    this->service->run();

    auto status = this->service->getStatus();
    this->websocketThread->addBroadcastMessage(status);

    this->loopMutex.unlock();
}

void ProcessingThread::stop() {
    this->stopped = true;
}

void ProcessingThread::setCycleInterval(int value) {
    this->cycleInterval = value;
}

void ProcessingThread::setMaxSpeed(double speed)
{
    this->loopMutex.lock();
    this->service->setMaxSpeed(speed);
    this->loopMutex.unlock();
}

void ProcessingThread::setDirection(RotationDirection direction)
{
    this->loopMutex.lock();
    this->service->setDirection(direction);
    this->loopMutex.unlock();
}

void ProcessingThread::setReleased(bool isReleased)
{
    this->loopMutex.lock();
    this->service->setReleased(isReleased);
    this->loopMutex.unlock();
}

void ProcessingThread::reloadUserConfig(UserSettings *settings)
{
    this->loopMutex.lock();
    this->service->loadUserConfig(settings);
    this->loopMutex.unlock();
}


