#ifndef GFORCE_TRAINER_CONTROLLER_PROCESSINGSERVICE_HPP
#define GFORCE_TRAINER_CONTROLLER_PROCESSINGSERVICE_HPP

#include <mutex>
#include "../Networking/MoviDriveService.hpp"
#include "../Networking/BusResponse.hpp"
#include "../Configuration/UserSettings.hpp"
#include "RotationDirection.hpp"
#include "Range.hpp"
#include "ProcessingStatus.hpp"
#include "Mode/OperationMode.hpp"
#include "Acceleration/AccelerationService.hpp"

using namespace GForce::Networking;
using namespace GForce::Configuration;
using namespace GForce::Processing::Mode;
using namespace GForce::Processing::Acceleration;

namespace GForce::Processing {

class ProcessingService
{
    private:
        MoviDriveService* driveService;
        AccelerationService* accelerationService;

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
        double maxSpeed;

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
         * Current operation mode
         */
        OperationMode* operationMode;

        /**
         * Syncs with MoviDrive
         */
        void sync(ControlStatus* controlStatus, double rotationSpeed);

        /**
         * Locked while updating or cloning status
         * Used for thread safe access to status
         */
        std::mutex statusMutex;

        /**
         * Calculates the target speed based on brake inputs
         */
        double calcTargetSpeed();

    public:
        ProcessingService(MoviDriveService *driveService, UserSettings *settings, AccelerationService *accelerationService);
        virtual ~ProcessingService();

        virtual void init();
        virtual void run();

        virtual void setFirstBrakeInput(int input);
        virtual void setSecondBrakeInput(int input);

        /**
         * Returns reflection of current processing status
         * Ownership of returned objects moves to caller
         */
        virtual ProcessingStatus* getStatus();

        void loadUserConfig(UserSettings* settings);
        void setMaxSpeed(double speed);
        void setDirection(RotationDirection value);
        void setReleased(bool isReleased);

        /**
         * Changes the operation mode, ownership of object moves to callee
         */
        void setOperationMode(OperationMode *mode);

        /**
         * Copies the status, ownership of returned object is moved to caller
         */
        BusResponse *cloneStatus();
};

}

#endif //GFORCE_TRAINER_CONTROLLER_PROCESSINGSERVICE_HPP
