#ifndef GFORCE_TRAINER_CONTROLLER_PROCESSINGTHREAD_HPP
#define GFORCE_TRAINER_CONTROLLER_PROCESSINGTHREAD_HPP

#include "ProcessingService.hpp"
#include "BrakeInputThread.hpp"
#include "../Configuration/UserSettings.hpp"
#include "../API/Websocket/ServerThread.hpp"

using namespace GForce::Configuration;
using namespace GForce::API::Websocket;

namespace GForce::Processing {

class ProcessingThread
{
    private:
        ProcessingService* service;

        BrakeInputThread* brakeInputThread;
        ServerThread* websocketThread;

        /**
         * Cycle interval in ms
         */
        int cycleInterval;

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

        void start(BrakeInputThread* brakeThread, ServerThread* serverThread);
        void stop();

        void reloadUserConfig(UserSettings* settings);
        void setMaxSpeed(int speed);
        void setDirection(RotationDirection direction);
        void setReleased(bool isReleased);

        void setCycleInterval(int value);
};

}

#endif //GFORCE_TRAINER_CONTROLLER_PROCESSINGTHREAD_HPP