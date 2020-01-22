#include "ServerThread.hpp"
#include "../../Utils/Logging/StandardLogger.hpp"

using namespace GForce::API::Websocket;

ServerThread::ServerThread(Server *server, LoggerInterface *logger) : server(server), logger(logger)
{
    this->stopped = false;
}

void ServerThread::start()
{
    this->stopped = false;
    this->run();
}

void ServerThread::run()
{
    std::unique_lock<std::mutex> lk(this->queueMutex);
    this->condition_variable.wait(lk, [this] {return !this->broadcastQueue.empty();});

    auto message = this->broadcastQueue.front();
    this->broadcastQueue.pop();
    this->sendMessage(message);

    lk.unlock();
    this->condition_variable.notify_one();
    if (!stopped) {
        this->run();
    }
}

void ServerThread::sendMessage(ResponseCastInterface *message)
{
    Response* response = nullptr;

    try {
        response = message->toResponse();
        this->server->broadcast(response);
    } catch (std::exception &e) {
        this->logger->error(LOG_CHANNEL_WEBSOCKET, "Failed broadcasting message => " + std::string(e.what()), {
                {"exceptionMessage", std::string(e.what())},
                {"responseType", (response != nullptr ? response->getType() : "null")},
        });
    }

    delete message;
    delete response;
}

void ServerThread::addBroadcastMessage(ResponseCastInterface *message)
{
    std::lock_guard<std::mutex> lk(this->queueMutex);
    this->broadcastQueue.push(message);
    this->condition_variable.notify_one();
}

void ServerThread::stop() {
    this->stopped = true;
}
