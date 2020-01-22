#include <thread>
#include "ProcessingThread.hpp"
#include "../Utils/Logging/StandardLogger.hpp"

using namespace GForce::Processing;

ProcessingThread::ProcessingThread(LoggerInterface* logger, ProcessingService *service) : service(service), logger(logger)
{
    this->brakeInputThread = nullptr;
    this->websocketThread = nullptr;
    this->cycleInterval = 20;
    this->statusInterval = 2;
    this->lastStatusSent = 0;
    this->stopped = false;

    this->brakeInputEmptyCount = 0;
    this->lastBrakeInputMessageCount = 0;
    this->brakeInputTimeoutThreshold = 50;
}

void ProcessingThread::start(GForce::Processing::BrakeInput::BrakeInputReceiveThread* brakeThread, Websocket::ServerThread* serverThread)
{
    this->brakeInputThread = brakeThread;
    this->websocketThread = serverThread;

    this->service->init();

    std::chrono::milliseconds period(this->cycleInterval);
    std::chrono::high_resolution_clock::time_point next;

    bool firstLoop = true;

    while (!stopped) {
        next = std::chrono::high_resolution_clock::now() + period;
        this->loop();
        std::this_thread::sleep_until(next);

        if (firstLoop) {
            this->logger->info(LOG_CHANNEL_PROCESSING, "First loop executed", {});
            firstLoop = false;
        }
    }
}

void ProcessingThread::loop()
{
    this->loopMutex.lock();
    int firstInput = this->brakeInputThread->getFirstBrake();
    int secondInput = this->brakeInputThread->getSecondBrake();
    int currentMessageCount = this->brakeInputThread->getMessageCount();

    // Increment empty cycles count if no new message arrived since last call
    this->brakeInputEmptyCount = currentMessageCount == this->lastBrakeInputMessageCount
            ? (this->brakeInputEmptyCount + 1)
            : 0;

    // Checking if timeout for brake input is reached
    if (this->brakeInputEmptyCount >= this->brakeInputTimeoutThreshold) {
        int lastFirstInput = firstInput;
        int lastSecondInput = secondInput;

        firstInput = 0;
        secondInput = 0;

        this->logger->error(LOG_CHANNEL_PROCESSING, "Reached timeout for brake input, setting brake values to zero", {
            {"lastBrakeInputMessageCount", this->lastBrakeInputMessageCount},
            {"brakeInputEmptyCount", this->brakeInputEmptyCount},
            {"brakeInputTimeoutThreshold", this->brakeInputTimeoutThreshold},
            {"lastFirstBrakeInput", lastFirstInput},
            {"lastSecondBrakeInput", lastSecondInput},
        });
    }

    this->service->setFirstBrakeInput(firstInput);
    this->service->setSecondBrakeInput(secondInput);
    this->service->run();

    if (this->lastStatusSent >= (this->statusInterval - 1)) {
        auto status = this->service->getStatus();
        this->websocketThread->addBroadcastMessage(status);
        this->lastStatusSent = 0;
    } else {
        this->lastStatusSent++;
    }

    this->lastBrakeInputMessageCount = currentMessageCount;
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

void ProcessingThread::setOperationMode(OperationMode *mode)
{
    this->loopMutex.lock();
    this->service->setOperationMode(mode);
    this->loopMutex.unlock();
}

void ProcessingThread::setStatusInterval(int interval) {
    this->statusInterval = interval;
}
