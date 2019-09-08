#ifndef GFORCE_TRAINER_CONTROLLER_BRAKEINPUTTRANSMISSIONTHREAD_HPP
#define GFORCE_TRAINER_CONTROLLER_BRAKEINPUTTRANSMISSIONTHREAD_HPP

#include "../../Sensors/ADCSensorInterface.hpp"
#include "../../Utils/Logging/LoggerInterface.hpp"
#include "../../ACL/TCP/TCPConnectionInterface.hpp"

using namespace GForce::Sensors;
using namespace GForce::Utils::Logging;
using namespace GForce::ACL::TCP;

namespace GForce::Processing::BrakeInput {

class BrakeInputTransmissionThread
{
    private:
        LoggerInterface* logger;
        ADCSensorInterface* sensor;

        TCPConnectionInterface* socket;

        /**
         * Is thread stopped?
         */
        bool stopped;

        void connect();
        void loop();

        static int scaleSignedInput(int value);

    public:
        BrakeInputTransmissionThread(LoggerInterface *logger, ADCSensorInterface *sensor);

        void start();
        void stop();

        void setSocket(TCPConnectionInterface *value);
};

}


#endif //GFORCE_TRAINER_CONTROLLER_BRAKEINPUTTRANSMISSIONTHREAD_HPP
