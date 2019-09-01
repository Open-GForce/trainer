#include "Server.hpp"

using namespace GForce::API::Websocket;

Server::Server()
{
    websocket.init_asio();

    websocket.set_open_handler(bind(&Server::on_open,this,::_1));
    websocket.set_close_handler(bind(&Server::on_close,this,::_1));
    websocket.set_message_handler(bind(&Server::on_message,this,::_1,::_2));
}

void Server::on_open(const connection_hdl& connection)
{
    connections.insert(connection);
}

void Server::on_close(const connection_hdl& connection)
{
    connections.erase(connection);
}

void Server::on_message(const connection_hdl& connection, const server::message_ptr& message)
{
    std::cout << "Message: " << message->get_payload() << "\n";
}

void Server::run(uint16_t port)
{
    websocket.listen(port);
    websocket.start_accept();
    websocket.run();
}

void Server::broadcast(Response *response)
{
    std::string data = response->toJSON().dump();

    for (const auto& connection : this->connections) {
        this->websocket.send(connection, data, websocketpp::frame::opcode::text);
    }
}
