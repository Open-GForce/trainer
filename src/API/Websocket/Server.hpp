#ifndef GFORCE_TRAINER_CONTROLLER_SERVER_HPP
#define GFORCE_TRAINER_CONTROLLER_SERVER_HPP

#include <set>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "ResponseCastInterface.hpp"
#include "RouterInterface.hpp"
#include "../../Utils/Logging/LoggerInterface.hpp"

typedef websocketpp::server<websocketpp::config::asio> server;

using namespace GForce::Utils::Logging;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

namespace GForce::API::Websocket {

class Server
{
    private:
        typedef std::set<connection_hdl,std::owner_less<connection_hdl>> con_list;

        RouterInterface* router;
        LoggerInterface* logger;

        server websocket;
        con_list connections;

    public:
        Server(RouterInterface *router, LoggerInterface *logger);

        /**
         * Sends a response to all active connections
         * Ownership of response object stays at caller
         */
        virtual void broadcast(Response* response);

        void onOpen(const connection_hdl &connection);
        void onClose(const connection_hdl &connection);
        void onMessage(const connection_hdl &connection, const server::message_ptr &message);
        void run(uint16_t port);
};

}

#endif //GFORCE_TRAINER_CONTROLLER_SERVER_HPP
