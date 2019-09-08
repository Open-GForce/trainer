#ifndef GFORCE_TRAINER_CONTROLLER_BOOSTTCPCONNECTION_HPP
#define GFORCE_TRAINER_CONTROLLER_BOOSTTCPCONNECTION_HPP

#include <boost/asio.hpp>
#include "TCPConnectionInterface.hpp"

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::endl;

namespace GForce::ACL::TCP {

class BoostTCPConnection : public TCPConnectionInterface
{
    private:
        tcp::socket socket;

        BoostTCPConnection(boost::asio::io_service& IOService, const std::string& address, int port);

    public:
        static BoostTCPConnection* connect(const std::string& address, int port);

        void send(const std::string& data) override;
};

}


#endif //GFORCE_TRAINER_CONTROLLER_BOOSTTCPCONNECTION_HPP
