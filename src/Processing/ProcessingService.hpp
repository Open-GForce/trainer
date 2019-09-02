#ifndef GFORCE_TRAINER_CONTROLLER_PROCESSINGSERVICE_HPP
#define GFORCE_TRAINER_CONTROLLER_PROCESSINGSERVICE_HPP

#include <mutex>
#include "../Networking/MoviDriveService.hpp"
#include "../Networking/BusResponse.hpp"
#include "../Configuration/UserSettings.hpp"
#include "RotationDirection.hpp"
#include "Range.hpp"
#include "ProcessingStatus.hpp"

using namespace GForce::Networking;
using namespace GForce::Configuration;

namespace GForce::Processing {

class ProcessingService
{
    private:
        MoviDriveService* driveService;

        /**
         * Release and start engine?
         */
        bool released;

        /**
         * Status of the engine
         */
        BusResponse* status;

        /**
         * Max. speed in 1/min
         */
        int maxSpeed;

        /**
         * Raw value of inner brake
         */
        int innerBrake;

        /**
         * Raw value of outer brake
         */
        int outerBrake;

        /**
         * Min/Max range for inner brake
         */
        Range* innerBrakeRange;

        /**
         * Min/Max range for outer brake
         */
        Range* outerBrakeRange;

        RotationDirection direction;

        /**
         * Syncs with MoviDrive
         */
        void sync(ControlStatus* controlStatus, double rotationSpeed);

        /**
         * Locked while updating or cloning status
         * Used for thread safe access to status access
         */
        std::mutex statusMutex;

        /**
         * Calculates the target speed based on brake inputs
         */
        double calcTargetSpeed();

    public:
        ProcessingService(MoviDriveService* driveService, UserSettings* settings);
        virtual ~ProcessingService();

        virtual void run();

        virtual void setFirstBrakeInput(int input);
        virtual void setSecondBrakeInput(int input);

        /**
         * Returns reflection of current processing status
         * Ownership of returned objects moves to caller
         */
        virtual ProcessingStatus* getStatus();

        void loadUserConfig(UserSettings* settings);
        void setMaxSpeed(int speed);
        void setDirection(RotationDirection value);
        void setReleased(bool isReleased);

        /**
         * Copies the status, ownership of returned object is moved to caller
         */
        BusResponse *cloneStatus();
};

}

#endif //GFORCE_TRAINER_CONTROLLER_PROCESSINGSERVICE_HPP
