#include "BoostTCPSocket.hpp"

using namespace GForce::ACL::TCP;

BoostTCPSocket::BoostTCPSocket(boost::asio::io_service& IOService, int port):
    socket(IOService),
    acceptor(IOService, tcp::endpoint(tcp::v4(), port ))
{
    this->acceptor.accept(this->socket);
}

BoostTCPSocket* BoostTCPSocket::listen(int port)
{
    boost::asio::io_service IOService;
    return new BoostTCPSocket(IOService, port);
}

std::string BoostTCPSocket::read()
{
    boost::asio::streambuf buf;
    boost::asio::read_until(this->socket, buf, "\n" );
    string data = boost::asio::buffer_cast<const char*>(buf.data());
    return data;
}
