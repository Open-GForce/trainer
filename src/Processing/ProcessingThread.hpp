#ifndef GFORCE_TRAINER_CONTROLLER_PROCESSINGTHREAD_HPP
#define GFORCE_TRAINER_CONTROLLER_PROCESSINGTHREAD_HPP

#include "ProcessingService.hpp"
#include "BrakeInput/IP/BrakeInputRXThread.hpp"
#include "../Configuration/UserSettings.hpp"
#include "../API/Websocket/ServerThread.hpp"

using namespace GForce::Configuration;
using namespace GForce::API;
using namespace GForce::Processing::BrakeInput;

namespace GForce::Processing {

class ProcessingThread
{
    private:
        LoggerInterface* logger;
        ProcessingService* service;

        BrakeInputThread* brakeInputThread;
        Websocket::ServerThread* websocketThread;

        /**
         * Cycle interval in ms
         */
        int cycleInterval;

        /**
         * Number of cycles without sending a cycle
         */
        int lastStatusSent;

        /**
         * Send status to UI every n cycles
         */
        int statusInterval;

        /**
         * Number of cycles without new brake input
         * If Threshold gets exceeded inputs will be set to zero
         */
        int brakeInputTimeoutThreshold;

        /**
         * Gets incremented on empty cycle
         * Gets reset as soon new brake input arrives
         */
        int brakeInputEmptyCount;

        /**
         * Contains last message count of BrakeInputReceiveThread
         */
        int lastBrakeInputMessageCount;

        /**
         * Is Thread stopped?
         */
        bool stopped;

        /**
         * Gets locked during loop rung for avoiding inter-thread race conditions
         */
        std::mutex loopMutex;

        void loop();

    public:
        explicit ProcessingThread(LoggerInterface* logger, ProcessingService *service);

        void start(BrakeInputThread* brakeThread, Websocket::ServerThread* serverThread);
        void stop();

        /**
         * Reloads the user configuration
         * Ownership of object stays at caller
         */
        virtual void reloadUserConfig(UserSettings* settings);

        virtual void setMaxSpeed(double speed);
        virtual void setDirection(RotationDirection direction);
        virtual void setReleased(bool isReleased);
        virtual void setOperationMode(OperationMode* mode);

        void setCycleInterval(int value);

        void setStatusInterval(int statusInterval);
};

}

#endif //GFORCE_TRAINER_CONTROLLER_PROCESSINGTHREAD_HPP
