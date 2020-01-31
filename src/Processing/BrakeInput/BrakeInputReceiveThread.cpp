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
    this->continuousFailureCount = 0;
    this->firstBrake = 0;
    this->secondBrake = 0;
    this->stopped = false;
    this->socket = nullptr;
    this->startMutex.lock();
}

void BrakeInputReceiveThread::start()
{
    this->listen();
    this->startMutex.unlock();

    while (!stopped) {
        try {
            auto data = this->socket->read();
            if (data.empty()) {continue;}

            auto message = BrakeInputMessage::fromJSON(nlohmann::json::parse(data));
            this->firstBrake = message.getFirstBrake();
            this->secondBrake = message.getSecondBrake();
            this->messageCount++;
            this->continuousFailureCount = 0;
        } catch (std::exception &e) {
            this->logger->error(LOG_CHANNEL_BRAKE_INPUT_RX, "Error while receiving brake input message => " + std::string(e.what()), {
                    {"messageCount", this->messageCount},
                    {"continuousFailureCount", this->continuousFailureCount},
                    {"lastFirstBrakeInput", this->firstBrake},
                    {"lastSecondBrakeInput", this->secondBrake},
                    {"exceptionMessage", std::string(e.what())}
            });
            usleep(10000);

            this->continuousFailureCount++;
            if (this->continuousFailureCount >= FAILURE_THRESHOLD) {
                this->logger->info(LOG_CHANNEL_BRAKE_INPUT_RX, "Error threshold exceeded, performing socket rebuild procedure.", {});
                this->closeSocket();
                this->listen();
            }
        }
    }

    this->closeSocket();
}

void BrakeInputReceiveThread::listen()
{
    if (this->socket == nullptr) {
        logger->info(LOG_CHANNEL_BRAKE_INPUT_RX, "Listening on port 8519 for brake inputs, waiting for connections", {});
        this->socket = BoostTCPSocket::listen(8519);
        logger->info(LOG_CHANNEL_BRAKE_INPUT_RX, "Client connected!", {});
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

void BrakeInputReceiveThread::closeSocket()
{
    if (this->socket == nullptr) {
        return;
    }

    this->logger->info(LOG_CHANNEL_BRAKE_INPUT_RX, "Trying to close socket", {});
    try {
        this->socket->close();
        this->socket = nullptr;
    } catch (std::exception &e) {
        this->logger->warning(LOG_CHANNEL_BRAKE_INPUT_RX, "Error while closing socket => " + std::string(e.what()), {
            {"exceptionMessage", std::string(e.what())}
        });
    }
    this->logger->info(LOG_CHANNEL_BRAKE_INPUT_RX, "Socket closed!", {});
}
