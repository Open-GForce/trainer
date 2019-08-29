#ifndef GFORCE_TRAINER_CONTROLLER_MOVIDRIVESERVICE_HPP
#define GFORCE_TRAINER_CONTROLLER_MOVIDRIVESERVICE_HPP

#include "ControlStatus.hpp"
#include "../ACL/CAN/CANSocket.hpp"
#include "../Utils/Logging/LoggerInterface.hpp"

using namespace GForce::ACL;
using namespace GForce::Utils::Logging;

namespace GForce::Networking {

class MoviDriveService
{
    const int CAN_TX_PDO_INDEX    = 0x182;
    const int CAN_SYNC_INDEX      = 0x080;
    const int CAN_HEARTBEAT_INDEX = 0x702;

    private:
        CAN::SocketInterface* socket;
        LoggerInterface* logger;

        /**
         * If this threshold is reached, soft brake is performed
         * Service will enter an infinity loop, sending brake control messages
         */
        int errorThreshold;

        /**
         * Count gets incremented on every exception during sync process
         * Gets reset if no exception occurred
         */
        int errorCount;

        /**
         * Send an heartbeat on <n> call to sync() function
         */
        int heartbeatInterval;

        /**
         * Number of calls to sync() method without heartbeat
         */
        int lastHeartbeat;

        /**
         * Control status sent with every sync() call
         */
        ControlStatus* controlStatus;

        /**
         * Engine rotation speed in 1/min, send with every sync() call
         */
        uint16_t rotationSpeed;

        /**
         * Sends the CAN PDOs
         */
        void send();

        /**
         * Receives the CAN status
         */
        void receive();

        /**
         * Sends heartbeat if needed
         */
        void handleHeartbeat();

        /**
         * Checks error count and performing emergency strategy if needed
         */
        void handleErrors();

        static uint8_t lowByte(uint16_t value);
        static uint8_t highByte(uint16_t value);

    public:
        explicit MoviDriveService(CAN::SocketInterface* socket, LoggerInterface* logger);
        virtual ~MoviDriveService();

        /**
         * Sends control status and rotation speed + heartbeat (if interval reached)
         * Receives and returns engine status report
         */
        void sync();

        /**
         * Ownership of control status moves to service (may be deleted anytime)
         */
        void setControlStatus(ControlStatus *status);
        void setRotationSpeed(uint16_t speed);

        /**
         * Is service in error state (no recovery possible)
         */
        bool networkingErrors();

        void setHeartbeatInterval(int interval);

        void resetErrorCount();
        void setErrorThreshold(int threshold);
};

}

#endif //GFORCE_TRAINER_CONTROLLER_MOVIDRIVESERVICE_HPP
