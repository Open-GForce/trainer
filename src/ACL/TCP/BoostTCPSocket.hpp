#ifndef GFORCE_TRAINER_CONTROLLER_BOOSTTCPSOCKET_HPP
#define GFORCE_TRAINER_CONTROLLER_BOOSTTCPSOCKET_HPP

#include "TCPSocketInterface.hpp"
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::endl;

namespace GForce::ACL::TCP {

class BoostTCPSocket : public TCPSocketInterface
{
    private:
        tcp::acceptor acceptor;
        tcp::socket socket;

        BoostTCPSocket(boost::asio::io_service& IOService, int port);

    public:
        static BoostTCPSocket* listen(int port);

        std::string read() override;
        void close() override;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_BOOSTTCPSOCKET_HPP
