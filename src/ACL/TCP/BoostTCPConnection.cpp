#include "BoostTCPConnection.hpp"
#include "../../Utils/Exceptions/RuntimeException.hpp"
#include "BrokenPipeException.hpp"

using namespace GForce::ACL::TCP;
using namespace GForce::Utils::Exceptions;

BoostTCPConnection::BoostTCPConnection(boost::asio::io_service& IOService, const std::string& address, int port): socket(IOService)
{
    socket.connect( tcp::endpoint( boost::asio::ip::address::from_string(address), port));
}

BoostTCPConnection* BoostTCPConnection::connect(const std::string& address, int port)
{
    boost::asio::io_service io_service;
    return new BoostTCPConnection(io_service, address, port);
}

void BoostTCPConnection::send(const std::string& data)
{
    boost::system::error_code error;
    boost::asio::write(this->socket, boost::asio::buffer(data + "\n"), error);

    if(error) {
        std::string message = "Sending TCP message failed (" + std::to_string(error.value()) + ") => " + error.message();

        if (error == boost::system::errc::broken_pipe) {
            throw BrokenPipeException(message, error.value());
        }
        throw RuntimeException(message, error.value());
    }
}

void BoostTCPConnection::close()
{
    this->socket.close();
}


