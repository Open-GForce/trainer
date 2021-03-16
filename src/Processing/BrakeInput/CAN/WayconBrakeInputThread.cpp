#include "WayconBrakeInputThread.hpp"
#include "../../../ACL/CAN/Message.hpp"

using namespace GForce::Processing::BrakeInput::CAN;
using namespace GForce::ACL::CAN;

WayconBrakeInputThread::WayconBrakeInputThread(CANThread *canThread,
                                               LoggerInterface *logger):
                                               canThread(canThread),
                                               stopped(false),
                                               logger(logger),
                                               firstBrake(0),
                                               secondBrake(0),
                                               messageCount(0),
                                               firstBrakeReceived(false),
                                               secondBrakeReceived(false)
{
    this->startMutex.lock();
}

void WayconBrakeInputThread::start()
{
    this->setOperational(NODE_FIRST_SENSOR);
    this->setOperational(NODE_SECOND_SENSOR);
    this->startMutex.unlock();

    while (!this->stopped) {
        this->loop();
    }
}

void WayconBrakeInputThread::loop()
{
    std::list<MessageInterface*> firstBrakeMessages = this->canThread->getFirstBrakeMessages();
    std::list<MessageInterface*> secondBrakeMessages = this->canThread->getSecondBrakeMessages();

    if (!firstBrakeMessages.empty()) {
        this->firstBrake = decodeMessage(firstBrakeMessages);
        this->firstBrakeReceived = true;
    }

    if (!secondBrakeMessages.empty()) {
        this->secondBrake = decodeMessage(secondBrakeMessages);
        this->secondBrakeReceived = true;
    }

    if (firstBrakeReceived && secondBrakeReceived) {
        this->messageCount++;
        this->firstBrakeReceived = false;
        this->secondBrakeReceived = false;
    }
}

int WayconBrakeInputThread::decodeMessage(std::list<MessageInterface*> messages)
{
    auto lastMessage = messages.back();
    auto bytes = lastMessage->getData();

    int brakeValue = (bytes[1]<<8) + bytes[0];

    for (auto message : messages) {
        delete message;
    }

    return brakeValue;
}

void WayconBrakeInputThread::setOperational(uint8_t node)
{
    auto message = new Message(0x0, {0x1, node});
    this->canThread->send(message);
}

void WayconBrakeInputThread::stop()
{
    this->stopped = true;
}

void WayconBrakeInputThread::waitUntilStarted()
{
    this->startMutex.unlock();
}

int WayconBrakeInputThread::getFirstBrake() const {
    return this->firstBrake;
}

int WayconBrakeInputThread::getSecondBrake() const {
    return this->secondBrake;
}

int WayconBrakeInputThread::getMessageCount() const {
    return this->messageCount;
}

