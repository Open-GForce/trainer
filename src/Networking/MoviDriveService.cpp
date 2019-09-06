#include <iostream>
#include <vector>
#include <zconf.h>
#include <cmath>
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

BusResponse* MoviDriveService::sync()
{
    BusResponse* response = nullptr;

    try {
        this->send();
        response = this->receive();
        this->handleHeartbeat();

        this->errorCount = 0;
    } catch (std::exception &e) {
        this->logger->error("Communication error: " + std::string(e.what()));
        this->errorCount++;
    }

    this->handleErrors();
    return response;
}


void MoviDriveService::send()
{
    uint16_t status = this->controlStatus->toBinary();

    std::vector<uint8_t> data = {0, 0, 0, 0, 0, 0};

    double floatSpeed = this->rotationSpeed < 0
            ? (65536 + (this->rotationSpeed * 5))
            : (this->rotationSpeed * 5);

    auto speed = (uint16_t) std::round(floatSpeed);

    data[0] = lowByte(status);
    data[1] = highByte(status);
    data[2] = lowByte(speed);
    data[3] = highByte(speed);

    auto pdoMessage = new CAN::Message(CAN_TX_PDO_INDEX, data);
    this->socket->send(pdoMessage);

    auto syncMessage = new CAN::Message(CAN_SYNC_INDEX, {});
    this->socket->send(syncMessage);
}

BusResponse* MoviDriveService::receive()
{
    auto messages = this->socket->receive();
    CAN::MessageInterface* lastMessage = nullptr;
    BusResponse* response = nullptr;

    // Filter for last status message, ignoring older and other messages
    for (auto message : messages) {
        if (message->getIndex() == CAN_RX_PDO_INDEX) {
            delete lastMessage;
            lastMessage = message;
        } else {
            delete message;
        }
    }

    if (lastMessage != nullptr) {
        response = BusResponse::fromMessage(lastMessage);
    }

    delete lastMessage;
    return response;
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

void MoviDriveService::setRotationSpeed(double speed) {
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
