#ifndef GFORCE_TRAINER_CONTROLLER_ADCSENSORINTERFACE_HPP
#define GFORCE_TRAINER_CONTROLLER_ADCSENSORINTERFACE_HPP

namespace GForce::Sensors {

class ADCSensorInterface
{
    public:
        virtual int read(unsigned channel) = 0;
};

}


#endif //GFORCE_TRAINER_CONTROLLER_ADCSENSORINTERFACE_HPP
