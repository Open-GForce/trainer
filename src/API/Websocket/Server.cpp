#include "Server.hpp"
#include "../../Utils/Exceptions/RuntimeException.hpp"
#include "../../Utils/Assertions/AssertionFailedException.hpp"
#include "../../Utils/Exceptions/InvalidArgumentException.hpp"

using namespace GForce::Utils::Exceptions;
using namespace GForce::Utils::Assertions;
using namespace GForce::API::Websocket;

Server::Server(RouterInterface *router, LoggerInterface *logger) : router(router), logger(logger)
{
    websocket.init_asio();

    websocket.clear_access_channels(websocketpp::log::alevel::all);

    websocket.set_open_handler(bind(&Server::onOpen,this,::_1));
    websocket.set_close_handler(bind(&Server::onClose,this,::_1));
    websocket.set_message_handler(bind(&Server::onMessage,this,::_1,::_2));
}

void Server::onOpen(const connection_hdl &connection)
{
    connections.insert(connection);
}

void Server::onClose(const connection_hdl &connection)
{
    connections.erase(connection);
}

void Server::onMessage(const connection_hdl &connection, const server::message_ptr &message)
{
    ResponseCastInterface* responseSource = nullptr;
    Response* response = nullptr;

    try {
        responseSource = this->router->handle(message->get_payload());

        if (responseSource != nullptr) {
            response = responseSource->toResponse();
            this->broadcast(response);
        }
    } catch (RuntimeException &e) {
        this->logger->error("Error[RuntimeException] while handling websocket request => " + e.getMessage());
    } catch (AssertionFailedException &e) {
        this->logger->error("Error[AssertionFailedException] while handling websocket request => " + e.getMessage());
    } catch (InvalidArgumentException &e) {
        this->logger->error("Error[InvalidArgumentException] while handling websocket request => " + e.getMessage());
    } catch (std::exception &e) {
        this->logger->error("Error[Unknown] while handling websocket request => " + std::string(e.what()));
    }

    delete responseSource;
    delete response;
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
