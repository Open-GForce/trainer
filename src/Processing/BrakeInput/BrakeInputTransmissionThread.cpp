#include "BrakeInputTransmissionThread.hpp"
#include "../../ACL/TCP/BoostTCPConnection.hpp"
#include "BrakeInputMessage.hpp"

using namespace GForce::Processing::BrakeInput;

BrakeInputTransmissionThread::BrakeInputTransmissionThread(LoggerInterface *logger, ADCSensorInterface *sensor): logger(logger), sensor(sensor)
{
    this->socket = nullptr;
    this->stopped = false;
}

void BrakeInputTransmissionThread::start()
{
    this->connect();

    while (!stopped) {
        try {
            this->loop();
        } catch (std::exception &e) {
            this->logger->error("Failure in BrakeInputTransmission thread => " + std::string(e.what()));
            usleep(10000);
        }
    }
}

void BrakeInputTransmissionThread::loop()
{
    int firstBrake = scaleSignedInput(this->sensor->read(0));
    int secondBrake = scaleSignedInput(this->sensor->read(1));

    auto message = BrakeInputMessage(firstBrake, secondBrake);
    this->socket->send(message.toJSON().dump());
}

void BrakeInputTransmissionThread::connect()
{
    while (this->socket == nullptr) {
        this->logger->info("Connecting to 127.0.0.1:8519");

        try {
            this->socket = BoostTCPConnection::connect("127.0.0.1", 8519);
            this->logger->info("Successfully connected!");
        } catch (std::exception &e) {
            this->logger->error("Connection failed => " + std::string(e.what()));
            this->socket = nullptr;
            sleep(1);
        }
    }
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
