#ifndef GFORCE_TRAINER_CONTROLLER_PROCESSINGSTATUS_HPP
#define GFORCE_TRAINER_CONTROLLER_PROCESSINGSTATUS_HPP

#include "RotationDirection.hpp"
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
        double maxSpeed;

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

        /**
         * Current rotation direction
         */
        RotationDirection rotationDirection;

        /**
         * String identifier of current operation mode
         */
        std::string operationMode;

    public:
        ProcessingStatus(EngineStatus *engineStatus, double rotationSpeed, double maxSpeed, double targetSpeed,
                         int innerBrakeRawValue, int outerBrakeRawValue, double innerBrakePercentage,
                         double outerBrakePercentage, RotationDirection rotationDirection, std::string operationMode);

        ~ProcessingStatus() override;

        Websocket::Response *toResponse() override;

        [[nodiscard]] EngineStatus *getEngineStatus() const;

        [[nodiscard]] double getRotationSpeed() const;
        [[nodiscard]] double getMaxSpeed() const;

        [[nodiscard]] double getTargetSpeed() const;

        [[nodiscard]] int getInnerBrakeRawValue() const;
        [[nodiscard]] int getOuterBrakeRawValue() const;

        [[nodiscard]] double getInnerBrakePercentage() const;
        [[nodiscard]] double getOuterBrakePercentage() const;

        [[nodiscard]] RotationDirection getRotationDirection() const;

        [[nodiscard]] const std::string &getOperationMode() const;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_PROCESSINGSTATUS_HPP
