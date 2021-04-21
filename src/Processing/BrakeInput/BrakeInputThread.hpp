#ifndef GFORCE_TRAINER_CONTROLLER_BRAKEINPUTTHREAD_H
#define GFORCE_TRAINER_CONTROLLER_BRAKEINPUTTHREAD_H

namespace GForce::Processing::BrakeInput {

/**
 * Interface for thread receiving brake input data
 */
class BrakeInputThread
{
    public:
        /**
         * Starts/Stops the thread
         */
        virtual void start() = 0 ;
        virtual void stop() = 0;

        /**
         * Blocks until thread has been started
         */
        virtual void waitUntilStarted() = 0;

        /**
         * Returns the raw brake data
         */
        [[nodiscard]] virtual int getFirstBrake() const = 0;
        [[nodiscard]] virtual int getSecondBrake() const = 0;

        /**
         * Returns the count of received messages since startup
         */
        [[nodiscard]] virtual int getMessageCount() const = 0;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_BRAKEINPUTTHREAD_H
