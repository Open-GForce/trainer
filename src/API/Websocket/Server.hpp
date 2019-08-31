#ifndef GFORCE_TRAINER_CONTROLLER_SERVER_HPP
#define GFORCE_TRAINER_CONTROLLER_SERVER_HPP

#include <set>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

namespace GForce::API::Websocket {

class Server
{
    private:
        typedef std::set<connection_hdl,std::owner_less<connection_hdl>> con_list;

        server websocket;
        con_list connections;

    public:
        Server();

        void on_open(const connection_hdl& connection);
        void on_close(const connection_hdl& connection);
        void on_message(const connection_hdl& connection, const server::message_ptr& message);
        void run(uint16_t port);
};

}

#endif //GFORCE_TRAINER_CONTROLLER_SERVER_HPP
