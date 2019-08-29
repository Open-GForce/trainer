#include <iostream>
#include <vector>
#include <zconf.h>
#include "MoviDriveService.hpp"
#include "../ACL/CAN/Message.hpp"

using namespace GForce::Networking;

MoviDriveService::MoviDriveService(CAN::SocketInterface* socket, LoggerInterface* logger): socket(socket), logger(logger)
{
    this->controlStatus = nullptr;
    this->rotationSpeed = 0;
    this->errorCount = 0;
    this->errorThreshold = 10;
    this->lastHeartbeat = 0;
    this->heartbeatInterval = 10;
}

MoviDriveService::~MoviDriveService()
{
    delete this->controlStatus;
}

void MoviDriveService::sync()
{
    try {
        this->send();
        this->receive();
        this->handleHeartbeat();

        this->errorCount = 0;
    } catch (std::exception &e) {
        this->logger->error("Communication error: " + std::string(e.what()));
        this->errorCount++;
    }

    this->handleErrors();
}


void MoviDriveService::send()
{
    uint16_t status = this->controlStatus->toBinary();

    std::vector<uint8_t> data = {0, 0, 0, 0};

    data[0] = lowByte(status);
    data[1] = highByte(status);
    data[2] = lowByte(rotationSpeed);
    data[3] = highByte(rotationSpeed);

    auto pdoMessage = new CAN::Message(CAN_TX_PDO_INDEX, data);
    this->socket->send(pdoMessage);

    auto syncMessage = new CAN::Message(CAN_SYNC_INDEX, {});
    this->socket->send(syncMessage);
}

void MoviDriveService::receive()
{

}

void MoviDriveService::handleHeartbeat()
{
    this->lastHeartbeat++;

    if (this->lastHeartbeat >= this->heartbeatInterval) {
        auto heartBeatMessage = new CAN::Message(CAN_HEARTBEAT_INDEX, {0x05});
        this->socket->send(heartBeatMessage);
        this->lastHeartbeat = 0;
    }
}

void MoviDriveService::handleErrors()
{
    while (this->errorCount >= this->errorThreshold) {
        this->setControlStatus(ControlStatus::softBrake());
        this->setRotationSpeed(0);

        try {
            this->send();
        } catch (std::exception &e) {}

        usleep(100000);
    }
}

uint8_t MoviDriveService::lowByte(uint16_t value) {
    return (uint8_t) (value & 0xff);
}

uint8_t MoviDriveService::highByte(uint16_t value) {
    return (uint8_t ) ((value >> 8) & 0xff);
}

void MoviDriveService::setControlStatus(ControlStatus *status) {
    delete this->controlStatus;
    this->controlStatus = status;
}

void MoviDriveService::setRotationSpeed(uint16_t speed) {
    this->rotationSpeed = speed;
}

bool MoviDriveService::networkingErrors() {
    return this->errorCount >= this->errorThreshold;
}

void MoviDriveService::setHeartbeatInterval(int interval) {
    this->heartbeatInterval = interval;
}

void MoviDriveService::resetErrorCount() {
    this->errorCount = 0;
}

void MoviDriveService::setErrorThreshold(int threshold) {
    this->errorThreshold = threshold;
}
