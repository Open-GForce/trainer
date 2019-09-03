#include <cstring>
#include <zconf.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include "CANSocket.hpp"
#include "../../Utils/Exceptions/RuntimeException.hpp"
#include "MessageFactory.hpp"

using namespace GForce::ACL::CAN;
using namespace GForce::Utils::Exceptions;

CANSocket::CANSocket()
{
    this->handle = -1;
}

void CANSocket::connect(const std::string &address, uint16_t port)
{
    this->handle = socket(AF_INET, SOCK_STREAM, 0);
    if (this->handle < 0) {
        throw RuntimeException("Unable to open CAN socket handle");
    }

    sockaddr_in server{};
    server.sin_addr.s_addr = inet_addr(address.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    int status = ::connect(this->handle, (struct sockaddr *) &server, sizeof(server));
    if (status < 0) {
        throw RuntimeException("Connection to socket failed (RC: " + std::to_string(status) + ")");
    }
}

void CANSocket::open()
{
    this->transfer("< open can0 >< rawmode >");
    usleep(50000);
    std::string response = this->fetch();

    if (response != "< hi >< ok >< ok >") {
        throw RuntimeException("Handshake failed. Response[" + std::to_string(response.size()) + "]: " + response);
    }
}

void CANSocket::send(MessageInterface *message)
{
    this->transfer(message->toFrame());
}

void CANSocket::transfer(const std::string& data)
{
    int status = ::send(this->handle, data.c_str(), strlen(data.c_str()), 0);

    if (status < 0) {
        throw RuntimeException("Data transfer failed (RC: " + std::to_string(status) + ")");
    }
}

std::string CANSocket::fetch()
{
    char buffer[512];

    int status = recv(this->handle, buffer, sizeof(buffer), 0);
    if (status < 0) {
        throw RuntimeException("CAN socket => call to recv() failed with RC " + std::to_string(status));
    }

    std::string data = buffer;
    int length = data.find_last_of('>') + 1;

    if (length > 0) {
        data = data.substr(0, length);
    }

    return data;
}

std::vector<MessageInterface *> CANSocket::receive()
{
    return MessageFactory::createFromString(this->fetch());
}


