#ifndef GFORCE_TRAINER_CONTROLLER_CANTHREAD_HPP
#define GFORCE_TRAINER_CONTROLLER_CANTHREAD_HPP

#include <list>
#include <mutex>
#include "../ACL/CAN/SocketInterface.hpp"
#include "../Utils/Logging/LoggerInterface.hpp"

using namespace GForce::ACL::CAN;
using namespace GForce::Utils::Logging;

namespace GForce::Networking {

class CANThread
{
    public:
        static const int inline CAN_SYNC_INDEX       = 0x080;
        static const int inline INDEX_MOVIDRIVE      = 0x182;
        static const int inline INDEX_BRAKE_SENSOR_1 = 0x100;
        static const int inline INDEX_BRAKE_SENSOR_2 = 0x200;

        /**
         * Stores not more then n messages per topic
         */
        static const int inline QUEUE_LENGTH         = 10;

    private:
        LoggerInterface* logger;
        SocketInterface* socket;

        /**
         * Cycle interval in ms
         */
        int cycleInterval;

        /**
         * Is thread stopped?
         */
        bool stopped;

        /**
         * Gets locked on access to messagesMoviDrive
         */
        std::mutex moviDriveMutex;

        /**
         * Gets locked on access to messagesFirstBrakeSensor or messagesSecondBrakeSensor
         */
        std::mutex brakeMutex;

        /**
         * Last messages seperated according to topic.
         * List length is limited by QUEUE_LENGTH
         */
        std::list<MessageInterface*> messagesMoviDrive;
        std::list<MessageInterface*> messagesFirstBrakeSensor;
        std::list<MessageInterface*> messagesSecondBrakeSensor;

        /**
         * Discards older messages for keeping queue length within limit
         */
        std::list<MessageInterface*> trimQueue(std::list<MessageInterface*> queue);

    public:
        CANThread(SocketInterface* socket, LoggerInterface *logger);

        /**
         * Starts the thread
         */
        void start();

        /**
         * Executes one
         */
        void loop();

        /**
         * Returns all received messages since last call.
         * Ownership of message objects moves to caller.
         */
        virtual std::list<MessageInterface*> getMoviDriveMessages();
        virtual std::list<MessageInterface*> getFirstBrakeMessages();
        virtual std::list<MessageInterface*> getSecondBrakeMessages();

        /**
         * Sends a message
         * Ownership of object moves to callee
         */
        virtual void send(MessageInterface* message);

};

}

#endif //GFORCE_TRAINER_CONTROLLER_CANTHREAD_HPP
