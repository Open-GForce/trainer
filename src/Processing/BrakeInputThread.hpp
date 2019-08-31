#ifndef GFORCE_TRAINER_CONTROLLER_BRAKEINPUTTHREAD_HPP
#define GFORCE_TRAINER_CONTROLLER_BRAKEINPUTTHREAD_HPP

#include "../Sensors/ADCSensorInterface.hpp"
#include "../Utils/Logging/LoggerInterface.hpp"

using namespace GForce::Sensors;
using namespace GForce::Utils::Logging;

namespace GForce::Processing {

class BrakeInputThread
{
    private:
        ADCSensorInterface* sensor;
        LoggerInterface* logger;

        int firstBrake;
        int secondBrake;

        bool stopped = false;

    public:
        BrakeInputThread(ADCSensorInterface *sensor, LoggerInterface *logger);

        void start();
        void stop();

        virtual int getFirstBrake() const;
        virtual int getSecondBrake() const;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_BRAKEINPUTTHREAD_HPP
