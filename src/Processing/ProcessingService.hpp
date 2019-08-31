#ifndef GFORCE_TRAINER_CONTROLLER_PROCESSINGSERVICE_HPP
#define GFORCE_TRAINER_CONTROLLER_PROCESSINGSERVICE_HPP

#include <mutex>
#include "../Networking/MoviDriveService.hpp"
#include "../Networking/Response.hpp"
#include "../Configuration/UserSettings.hpp"
#include "Range.hpp"

using namespace GForce::Networking;
using namespace GForce::Configuration;

namespace GForce::Processing {

enum RotationDirection{right, left};

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
        Response* status;

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

    public:
        ProcessingService(MoviDriveService* driveService, UserSettings* settings);
        virtual ~ProcessingService();

        void run();

        void setFirstBrakeInput(int input);
        void setSecondBrakeInput(int input);

        void setMaxSpeed(int speed);
        void setDirection(RotationDirection value);

        void setReleased(bool isReleased);

        /**
         * Copies the status, ownership of returned object is moved to caller
         */
        Response *cloneStatus();
};

}

#endif //GFORCE_TRAINER_CONTROLLER_PROCESSINGSERVICE_HPP
