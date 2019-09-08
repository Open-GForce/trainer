#ifndef GFORCE_TRAINER_CONTROLLER_PROCESSINGTHREAD_HPP
#define GFORCE_TRAINER_CONTROLLER_PROCESSINGTHREAD_HPP

#include "ProcessingService.hpp"
#include "BrakeInput/BrakeInputReceiveThread.hpp"
#include "../Configuration/UserSettings.hpp"
#include "../API/Websocket/ServerThread.hpp"

using namespace GForce::Configuration;
using namespace GForce::API;

namespace GForce::Processing {

class ProcessingThread
{
    private:
        ProcessingService* service;

        GForce::Processing::BrakeInput::BrakeInputReceiveThread* brakeInputThread;
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
         * Is Thread stopped?
         */
        bool stopped;

        /**
         * Gets locked during loop rung for avoiding inter-thread race conditions
         */
        std::mutex loopMutex;

        void loop();

    public:
        explicit ProcessingThread(ProcessingService *service);

        void start(GForce::Processing::BrakeInput::BrakeInputReceiveThread* brakeThread, Websocket::ServerThread* serverThread);
        void stop();

        /**
         * Reloads the user configuration
         * Ownership of object stays at caller
         */
        virtual void reloadUserConfig(UserSettings* settings);

        virtual void setMaxSpeed(double speed);
        virtual void setDirection(RotationDirection direction);
        virtual void setReleased(bool isReleased);

        void setCycleInterval(int value);

        void setStatusInterval(int statusInterval);
};

}

#endif //GFORCE_TRAINER_CONTROLLER_PROCESSINGTHREAD_HPP
