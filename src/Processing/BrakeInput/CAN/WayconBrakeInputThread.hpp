#ifndef GFORCE_TRAINER_CONTROLLER_WAYCONBRAKEINPUTTHREAD_HPP
#define GFORCE_TRAINER_CONTROLLER_WAYCONBRAKEINPUTTHREAD_HPP

#include "../BrakeInputThread.hpp"
#include "../../../Networking/CANThread.hpp"

using namespace GForce::Networking;
using namespace GForce::Utils::Logging;

namespace GForce::Processing::BrakeInput::CAN {

class WayconBrakeInputThread : public BrakeInputThread
{
    public:
        static const uint8_t inline NODE_FIRST_SENSOR  = 0x07;
        static const uint8_t inline NODE_SECOND_SENSOR = 0x08;

    private:
        CANThread* canThread;
        LoggerInterface* logger;

        /**
         * Gets unlocked on startup
         */
        std::mutex startMutex;

        /**
         * Is thread stopped?
         */
        bool stopped;

        /**
         * Last raw values received
         */
        int firstBrake;
        int secondBrake;

        /**
         * Combined message counts
         * Gets incremented when new message of BOTH brakes has been received
         */
        int messageCount;

        /**
         * Flag if new message received since last messter counter increment
         */
        bool firstBrakeReceived;
        bool secondBrakeReceived;

        /**
         * Sets the given sensor to operational mode
         */
        void setOperational(uint8_t node);

        /**
         * Decodes the given message list to raw brake input value
         */
        int decodeMessage(std::list<MessageInterface*> messages);

    public:
        WayconBrakeInputThread(CANThread *canThread, LoggerInterface *logger);

        void start() override;
        void stop() override;
        void waitUntilStarted() override;

        /**
         * Executes one cycle
         * Only used in unittests
         */
        void loop();

        int getFirstBrake() const override;
        int getSecondBrake() const override;
        int getMessageCount() const override;

};

}


#endif //GFORCE_TRAINER_CONTROLLER_WAYCONBRAKEINPUTTHREAD_HPP
