#include <chrono>
#include <thread>
#include "CANThread.hpp"
#include "../Utils/Logging/StandardLogger.hpp"

using namespace GForce::Networking;

CANThread::CANThread(SocketInterface* socket, LoggerInterface* logger)
        : socket(socket), stopped(false), cycleInterval(10), logger(logger) {}

void CANThread::start()
{
    this->stopped = false;

    std::chrono::milliseconds period(this->cycleInterval);
    std::chrono::high_resolution_clock::time_point next;

    bool firstLoop = true;

    while (!stopped) {
        next = std::chrono::high_resolution_clock::now() + period;
        try {
            this->loop();
        } catch (std::exception &e) {
            this->logger->error(LOG_CHANNEL_CAN, "Error while executing CAN thread loop => " + std::string(e.what()), {});
        }
        std::this_thread::sleep_until(next);

        if (firstLoop) {
            this->logger->info(LOG_CHANNEL_CAN, "First loop executed", {});
            firstLoop = false;
        }
    }
}

void CANThread::loop()
{
    std::vector<MessageInterface*> messages = this->socket->receive();

    this->moviDriveMutex.lock();
    this->brakeMutex.lock();

    for (MessageInterface* message : messages) {
        switch (message->getIndex()) {
            case INDEX_MOVIDRIVE:
                this->messagesMoviDrive.push_back(message); break;
            case INDEX_BRAKE_SENSOR_1:
                this->messagesFirstBrakeSensor.push_back(message); break;
            case INDEX_BRAKE_SENSOR_2:
                this->messagesSecondBrakeSensor.push_back(message); break;
            default:
                delete message;
        }
    }

    this->messagesMoviDrive = trimQueue(this->messagesMoviDrive);
    this->messagesFirstBrakeSensor = trimQueue(this->messagesFirstBrakeSensor);
    this->messagesSecondBrakeSensor = trimQueue(this->messagesSecondBrakeSensor);

    this->moviDriveMutex.unlock();
    this->brakeMutex.unlock();
}

std::list<MessageInterface*> CANThread::trimQueue(std::list<MessageInterface*> queue)
{
    int size = queue.size();

    while (queue.size() > QUEUE_LENGTH) {
        MessageInterface* message = queue.front();
        delete message;
        queue.pop_front();
    }

    return queue;
}

std::list<MessageInterface *> CANThread::getMoviDriveMessages()
{
    this->moviDriveMutex.lock();
    auto messages = this->messagesMoviDrive;
    this->messagesMoviDrive.clear();
    this->moviDriveMutex.unlock();

    return messages;
}

std::list<MessageInterface *> CANThread::getFirstBrakeMessages()
{
    this->brakeMutex.lock();
    auto messages = this->messagesFirstBrakeSensor;
    this->messagesFirstBrakeSensor.clear();
    this->brakeMutex.unlock();

    return messages;
}

std::list<MessageInterface *> CANThread::getSecondBrakeMessages()
{
    this->brakeMutex.lock();
    auto messages = this->messagesSecondBrakeSensor;
    this->messagesSecondBrakeSensor.clear();
    this->brakeMutex.unlock();

    return messages;
}

void CANThread::send(MessageInterface *message)
{
    this->socket->send(message);
}
