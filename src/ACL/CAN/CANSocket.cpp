#include <cstring>
#include <zconf.h>
#include "CANSocket.hpp"
#include "../../Utils/Exceptions/RuntimeException.hpp"

extern  "C"
{
#include "../../../../can-utils/lib.h"
}

using namespace GForce::ACL::CAN;
using namespace GForce::Utils::Exceptions;

void CANSocket::open()
{
    this->rawSocket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (this->rawSocket < 0) {
        throw RuntimeException("Unable to initialize CAN socket");
    }

    this->socketCANAddress.can_family = AF_CAN;
    strcpy(this->frequency.ifr_name, "can0");

    if (ioctl(this->rawSocket, SIOCGIFINDEX, &this->frequency)) {
        throw RuntimeException("Error while selecting can0 interface");
    }

    if (bind(this->rawSocket, (struct sockaddr *) &this->socketCANAddress, sizeof(this->socketCANAddress)) < 0) {
        throw RuntimeException("Error while binding can0 interface");
    }
}

void CANSocket::send(Message *message)
{
    canfd_frame frame{};

    std::string encoded = message->toString();
    char buffer[encoded.size() + 1];
    strcpy(buffer, encoded.c_str());

    parse_canframe(buffer, &frame);

    int bytesWritten = write(this->rawSocket, &frame, sizeof(frame));
    if (bytesWritten != sizeof(frame)) {
        throw RuntimeException("Error while transferring CAN frame");
    }
}


