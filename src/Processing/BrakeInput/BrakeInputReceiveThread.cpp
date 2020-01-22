#include <string>
#include "BrakeInputReceiveThread.hpp"
#include <exception>
#include "../../ACL/TCP/BoostTCPSocket.hpp"
#include "BrakeInputMessage.hpp"
#include "../../Utils/Logging/StandardLogger.hpp"

using namespace GForce::Processing::BrakeInput;

BrakeInputReceiveThread::BrakeInputReceiveThread(LoggerInterface *logger): logger(logger)
{
    this->messageCount = 0;
    this->firstBrake = 0;
    this->secondBrake = 0;
    this->stopped = false;
    this->socket = nullptr;
    this->startMutex.lock();
}

void BrakeInputReceiveThread::start()
{
    if (this->socket == nullptr) {
        logger->info(LOG_CHANNEL_BRAKE_INPUT_RX, "Listening on port 8519 for brake inputs, waiting for connections", {});
        this->socket = BoostTCPSocket::listen(8519);
        logger->info(LOG_CHANNEL_BRAKE_INPUT_RX, "Client connected!", {});
    }

    this->startMutex.unlock();
    while (!stopped) {
        try {
            auto data = this->socket->read();
            if (data.empty()) {continue;}

            auto message = BrakeInputMessage::fromJSON(nlohmann::json::parse(data));
            this->firstBrake = message.getFirstBrake();
            this->secondBrake = message.getSecondBrake();
            this->messageCount++;
        } catch (std::exception &e) {
            this->logger->error(LOG_CHANNEL_BRAKE_INPUT_RX, "Error while receiving brake input message => " + std::string(e.what()), {
                    {"messageCount", this->messageCount},
                    {"lastFirstBrakeInput", this->firstBrake},
                    {"lastSecondBrakeInput", this->secondBrake},
                    {"exceptionMessage", std::string(e.what())}
            });
        }
    }
}

void BrakeInputReceiveThread::waitUntilStarted()
{
    this->startMutex.lock();
}

void BrakeInputReceiveThread::setSocket(TCPSocketInterface *value) {
    this->socket = value;
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

int BrakeInputReceiveThread::getMessageCount() const {
    return messageCount;
}