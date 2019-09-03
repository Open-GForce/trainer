#include "DummyCANSocket.hpp"
#include "Message.hpp"

using namespace GForce::ACL::CAN;

void DummyCANSocket::send(GForce::ACL::CAN::MessageInterface *message)
{
}

std::vector<MessageInterface *> DummyCANSocket::receive()
{
    return {
            new CAN::Message(0x182, {0x07, 0x00, 0x1c, 0x03})
    };
}
