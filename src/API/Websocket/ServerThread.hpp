#ifndef GFORCE_TRAINER_CONTROLLER_SERVERTHREAD_HPP
#define GFORCE_TRAINER_CONTROLLER_SERVERTHREAD_HPP

#include <queue>
#include "../../Utils/Logging/LoggerInterface.hpp"
#include "Server.hpp"
#include "ResponseCastInterface.hpp"

using namespace GForce::Utils::Logging;

namespace GForce::API::Websocket {

class ServerThread
{
    private:
        Server* server;
        LoggerInterface* logger;

        /**
         * Queue of broadcast messages to sent
         */
        std::queue<ResponseCastInterface*> broadcastQueue;

        /**
         * Threads get waked up as soon new messages arrive
         */
        std::mutex queueMutex;
        std::condition_variable condition_variable;

        /**
         * Waits for new messages and sends them to clients
         */
        void run();

        /**
         * Sends message to all clients
         * Ownership of message moves to method
         */
        void sendMessage(ResponseCastInterface* message);

        /**
         * Is thread stopped?
         */
        bool stopped;

    public:
        ServerThread(Server *server, LoggerInterface *logger);

        /**
         * Add a broadcast message to queue
         * Ownership of message moves to thread
         */
        virtual void addBroadcastMessage(ResponseCastInterface* message);

        void start();
        void stop();
};

}

#endif //GFORCE_TRAINER_CONTROLLER_SERVERTHREAD_HPP
