#ifndef GFORCE_TRAINER_CONTROLLER_PROCESSINGSTATUS_HPP
#define GFORCE_TRAINER_CONTROLLER_PROCESSINGSTATUS_HPP

#include "../API/Websocket/ResponseCastInterface.hpp"
#include "../Networking/EngineStatus.hpp"

using namespace GForce::Networking;
using namespace GForce::API;

namespace GForce::Processing
{

/**
 * Represents the processing status
 * Reflects properties of ProcessingService
 */
class ProcessingStatus : public Websocket::ResponseCastInterface
{
    private:
        /**
         * Current engine status
         */
        EngineStatus* engineStatus;

        /**
         * Current rotation speed in 1/min
         */
        double rotationSpeed;

        /**
         * Max. configured rotation speed in 1/min
         */
        int maxSpeed;

        /**
         * Target speed based on brake calculation
         */
        double targetSpeed;

        /**
         * Raw brake input values
         */
        int innerBrakeRawValue;
        int outerBrakeRawValue;

        /**
         * Scaled brake values (0 - 100%)
         */
        double innerBrakePercentage;
        double outerBrakePercentage;

    public:
        ProcessingStatus(EngineStatus *engineStatus, double rotationSpeed, int maxSpeed, double targetSpeed,
                         int innerBrakeRawValue, int outerBrakeRawValue, double innerBrakePercentage,
                         double outerBrakePercentage);

        virtual ~ProcessingStatus();

        Websocket::Response *toResponse() override;

        EngineStatus *getEngineStatus() const;

        double getRotationSpeed() const;
        int getMaxSpeed() const;

        double getTargetSpeed() const;

        int getInnerBrakeRawValue() const;
        int getOuterBrakeRawValue() const;

        double getInnerBrakePercentage() const;
        double getOuterBrakePercentage() const;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_PROCESSINGSTATUS_HPP
