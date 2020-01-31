#include <cmath>
#include "BrakeInputTransmissionThread.hpp"
#include "../../ACL/TCP/BoostTCPConnection.hpp"
#include "BrakeInputMessage.hpp"
#include "../../Utils/Logging/StandardLogger.hpp"
#include "../../Utils/Exceptions/RuntimeException.hpp"
#include "../../Utils/Algorithms.hpp"
#include "../../ACL/TCP/BrokenPipeException.hpp"

using namespace GForce::Processing::BrakeInput;
using namespace GForce::Utils::Exceptions;
using namespace GForce::Utils;

BrakeInputTransmissionThread::BrakeInputTransmissionThread(LoggerInterface *logger, ADCSensorInterface *sensor): logger(logger), sensor(sensor)
{
    this->socket = nullptr;
    this->stopped = false;
    this->mainControllerAddress = "192.168.2.201";

    this->firstBrakeHistory = {};
    this->secondBrakeHistory = {};
    this->normalizeLength = 15;
}

void BrakeInputTransmissionThread::start()
{
    this->connect();

    while (!stopped) {
        try {
            this->loop();
        } catch (BrokenPipeException &e) {
            this->logger->error(LOG_CHANNEL_BRAKE_INPUT_TX, "Detected broken pipe, performing reconnect procedure.", this->getErrorContext(e.getMessage()));
            this->reconnect();
        } catch (RuntimeException &e) {
            auto context = this->getErrorContext(e.getMessage());
            context["errorCode"] = e.getCode();
            this->logger->error(LOG_CHANNEL_BRAKE_INPUT_TX, "Generic Failure in BrakeInputTransmission thread => " + e.getMessage(), context);
            usleep(10000);
        } catch (std::exception &e) {
            this->logger->error(LOG_CHANNEL_BRAKE_INPUT_TX, "Generic Failure in BrakeInputTransmission thread => " + std::string(e.what()), this->getErrorContext(std::string(e.what())));
            usleep(10000);
        }
    }
}

void BrakeInputTransmissionThread::loop()
{
    int firstBrake = scaleSignedInput(this->sensor->read(0));
    int secondBrake = scaleSignedInput(this->sensor->read(1));

    this->firstBrakeHistory.push_back(firstBrake);
    this->secondBrakeHistory.push_back(secondBrake);

    if (this->firstBrakeHistory.size() > this->normalizeLength) {
        this->firstBrakeHistory.erase(this->firstBrakeHistory.begin());
        this->secondBrakeHistory.erase(this->secondBrakeHistory.begin());
    }

    firstBrake = calcAverage(this->firstBrakeHistory);
    secondBrake = calcAverage(this->secondBrakeHistory);

    auto message = BrakeInputMessage(firstBrake, secondBrake);
    this->socket->send(message.toJSON().dump());
}

void BrakeInputTransmissionThread::connect()
{
    while (this->socket == nullptr) {
        this->logger->setGlobalContext("mainControllerAddress", this->mainControllerAddress);
        this->logger->info(LOG_CHANNEL_BRAKE_INPUT_TX, "Connecting to " + this->mainControllerAddress + ":8519", {});

        try {
            this->socket = BoostTCPConnection::connect(this->mainControllerAddress, 8519);
            this->logger->info(LOG_CHANNEL_BRAKE_INPUT_TX, "Successfully connected!", {});
        } catch (std::exception &e) {
            this->logger->error(LOG_CHANNEL_BRAKE_INPUT_TX, "Connection failed => " + std::string(e.what()), {{"exceptionMessage", std::string(e.what())}});
            delete this->socket;
            this->socket = nullptr;
            sleep(1);
        }
    }
}

void BrakeInputTransmissionThread::reconnect()
{
    if (this->socket != nullptr) {
        try {
            this->logger->info(LOG_CHANNEL_BRAKE_INPUT_TX, "Trying to close socket", {});
            this->socket->close();
        } catch (std::exception &e) {
            this->logger->warning(LOG_CHANNEL_BRAKE_INPUT_TX, "Error while closing socket => " + std::string(e.what()), {
                {"exceptionMessage", std::string(e.what())}
            });
        }
        delete this->socket;
        this->socket = nullptr;
    }

    this->connect();
}

int BrakeInputTransmissionThread::scaleSignedInput(int value)
{
    return value > 32768 ? (0 - (65536 - value)) : value;
}

void BrakeInputTransmissionThread::stop() {
    this->stopped = true;
}

void BrakeInputTransmissionThread::setSocket(TCPConnectionInterface *value){
    this->socket = value;
}

int BrakeInputTransmissionThread::calcAverage(const std::vector<int>& values)
{
    double sum = 0;

    for (int value : values) {
        sum += value;
    }

    return (int) round(sum / values.size());
}


nlohmann::json BrakeInputTransmissionThread::getErrorContext(std::string errorMessage) {
    return {
        {"exceptionMessage", errorMessage},
        {"normalizationLength", this->normalizeLength},
        {"firstBrakeHistory", Algorithms::implodeVector(this->firstBrakeHistory)},
        {"secondBrakeHistory", Algorithms::implodeVector(this->secondBrakeHistory)},
    };
}