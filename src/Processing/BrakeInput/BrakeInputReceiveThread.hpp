#ifndef GFORCE_TRAINER_CONTROLLER_BRAKEINPUTRECEIVETHREAD_HPP
#define GFORCE_TRAINER_CONTROLLER_BRAKEINPUTRECEIVETHREAD_HPP

#include <mutex>
#include "../../Utils/Logging/LoggerInterface.hpp"
#include "../../ACL/TCP/TCPSocketInterface.hpp"

using namespace GForce::ACL::TCP;
using namespace GForce::Utils::Logging;

namespace GForce::Processing::BrakeInput {

class BrakeInputReceiveThread
{
    private:
        /**
         * If continuousFailureCount exceeds this limit, sockets gets closed and reopened
         */
        const static int FAILURE_THRESHOLD = 50;

        LoggerInterface* logger;
        TCPSocketInterface* socket;

        int firstBrake;
        int secondBrake;

        bool stopped = false;

        /**
         * Gets incremented on every received message
         */
        int messageCount;

        /**
         * Get incremented on every errors, gets reset on successful loop
         */
        int continuousFailureCount;

        /**
         * Will be unlocked when started
         */
        std::mutex startMutex;

        /**
         * Opens socket and listens for incoming connections
         */
        void listen();

        /**
         * Tries to close the socket, catches and logs exceptions
         */
        void closeSocket();

    public:
        explicit BrakeInputReceiveThread(LoggerInterface *logger);

        void start();
        void stop();
        void waitUntilStarted();

        [[nodiscard]] virtual int getFirstBrake() const;
        [[nodiscard]] virtual int getSecondBrake() const;
        [[nodiscard]] virtual int getMessageCount() const;

        void setSocket(TCPSocketInterface *value);
};

}

#endif //GFORCE_TRAINER_CONTROLLER_BRAKEINPUTRECEIVETHREAD_HPP
