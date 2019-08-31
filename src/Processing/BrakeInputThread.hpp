#ifndef GFORCE_TRAINER_CONTROLLER_BRAKEINPUTTHREAD_HPP
#define GFORCE_TRAINER_CONTROLLER_BRAKEINPUTTHREAD_HPP

#include "../Sensors/ADCSensorInterface.hpp"

using namespace GForce::Sensors;

namespace GForce::Processing {

class BrakeInputThread
{
    private:
        ADCSensorInterface* sensor;

        int firstBrake;
        int secondBrake;

        bool stopped = false;

    public:
        explicit BrakeInputThread(ADCSensorInterface *sensor);

        void start();
        void stop();

        int getFirstBrake() const;
        int getSecondBrake() const;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_BRAKEINPUTTHREAD_HPP
