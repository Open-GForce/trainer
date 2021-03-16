#include <string>
#include "BrakeInputRXThread.hpp"
#include <exception>
#include "../../../ACL/TCP/BoostTCPSocket.hpp"
#include "BrakeInputMessage.hpp"
#include "../../../Utils/Logging/StandardLogger.hpp"

using namespace GForce::Processing::BrakeInput::IP;

BrakeInputRXThread::BrakeInputRXThread(LoggerInterface *logger): logger(logger)
{
    this->messageCount = 0;
    this->continuousFailureCount = 0;
    this->failureThreshold = DEFAULT_FAILURE_THRESHOLD;
    this->firstBrake = 0;
    this->secondBrake = 0;
    this->stopped = false;
    this->socket = nullptr;
    this->factory = new BoostTCPSocketFactory();
    this->startMutex.lock();
}

void BrakeInputRXThread::start()
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
            if (this->continuousFailureCount >= this->failureThreshold) {
                this->logger->info(LOG_CHANNEL_BRAKE_INPUT_RX, "Error threshold exceeded, performing socket rebuild procedure.", {});
                this->closeSocket();
                this->listen();
            }
        }
    }

    this->closeSocket();
}

void BrakeInputRXThread::listen()
{
    if (this->socket == nullptr) {
        logger->info(LOG_CHANNEL_BRAKE_INPUT_RX, "Listening on port 8519 for brake inputs, waiting for connections", {});
        this->socket = this->factory->listen(8519);
        logger->info(LOG_CHANNEL_BRAKE_INPUT_RX, "Client connected!", {});
    }
}

void BrakeInputRXThread::waitUntilStarted()
{
    this->startMutex.lock();
}

void BrakeInputRXThread::setSocket(TCPSocketInterface *value) {
    this->socket = value;
}

void BrakeInputRXThread::setFactory(TCPSocketFactory *socketFactory) {
    delete this->factory;
    this->factory = socketFactory;
}

void BrakeInputRXThread::setFailureThreshold(int threshold) {
    this->failureThreshold = threshold;
}

void BrakeInputRXThread::stop() {
    this->stopped = true;
}

int BrakeInputRXThread::getFirstBrake() const {
    return firstBrake;
}

int BrakeInputRXThread::getSecondBrake() const {
    return secondBrake;
}

int BrakeInputRXThread::getMessageCount() const {
    return messageCount;
}

void BrakeInputRXThread::closeSocket()
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
