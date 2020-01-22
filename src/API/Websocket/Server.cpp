#include "Server.hpp"
#include "../../Utils/Exceptions/RuntimeException.hpp"
#include "../../Utils/Assertions/AssertionFailedException.hpp"
#include "../../Utils/Exceptions/InvalidArgumentException.hpp"
#include "../../Utils/Logging/StandardLogger.hpp"

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
        this->logException("RuntimeException", e.getMessage(), message->get_payload());
    } catch (AssertionFailedException &e) {
        this->logException("AssertionFailedException", e.getMessage(), message->get_payload());
    } catch (InvalidArgumentException &e) {
        this->logException("InvalidArgumentException", e.getMessage(), message->get_payload());
    } catch (std::exception &e) {
        this->logException("Unknown", std::string(e.what()), message->get_payload());
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

void Server::logException(const std::string& exceptionClass, const std::string &exceptionMessage, const std::string &requestPayload)
{
    this->logger->error(LOG_CHANNEL_WEBSOCKET, "Error[" + exceptionClass + "] while handling websocket request => " + exceptionMessage, {
            {"exceptionClass", exceptionClass},
            {"exceptionMessage", exceptionMessage},
            {"requestPayload", requestPayload}
    });
}
