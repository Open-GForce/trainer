#ifndef GFORCE_TRAINER_CONTROLLER_PROCESSINGTHREAD_HPP
#define GFORCE_TRAINER_CONTROLLER_PROCESSINGTHREAD_HPP

#include "ProcessingService.hpp"
#include "BrakeInputThread.hpp"

namespace GForce::Processing {

class ProcessingThread
{
    private:
        ProcessingService* service;

        BrakeInputThread* brakeInputThread;

        /**
         * Cycle interval in ms
         */
        int cycleInterval;

        /**
         * Is Thread stopped?
         */
        bool stopped;

        void loop();

    public:
        explicit ProcessingThread(ProcessingService *service);

        void start(BrakeInputThread* brakeThread);
        void stop();

        void setCycleInterval(int value);
};

}

#endif //GFORCE_TRAINER_CONTROLLER_PROCESSINGTHREAD_HPP
