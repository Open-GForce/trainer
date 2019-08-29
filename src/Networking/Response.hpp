#ifndef GFORCE_TRAINER_CONTROLLER_RESPONSE_HPP
#define GFORCE_TRAINER_CONTROLLER_RESPONSE_HPP

#include "EngineStatus.hpp"
#include "../ACL/CAN/MessageInterface.hpp"

using namespace GForce::ACL;

namespace GForce::Networking
{

class Response
{
    private:
        /**
         * MoviDrive engine status
         * s. 7.4.6 of https://download.sew-eurodrive.com/download/pdf/11264918.pdf
         */
        EngineStatus* engineStatus;

        /**
         * Current engine roation speed in 1/min
         */
        int rotationSpeed;

    public:
        Response(EngineStatus *engineStatus, int rotationSpeed);

        virtual ~Response();

        /**
         * Converts CAN message to Response
         */
        static Response* fromMessage(CAN::MessageInterface* message);

        EngineStatus *getEngineStatus() const;
        int getRotationSpeed() const;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_RESPONSE_HPP
