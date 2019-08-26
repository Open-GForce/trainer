#ifndef GFORCE_TRAINER_CONTROLLER_ADS1115_HPP
#define GFORCE_TRAINER_CONTROLLER_ADS1115_HPP

#include "../ACL/I2C/Device.hpp"

using namespace GForce::ACL::I2C;

namespace GForce::Sensors {

class ADS1115
{
    private:
        Device* device;

    public:
        explicit ADS1115(Device *device);

        double read(unsigned channel);
};

}

#endif //GFORCE_TRAINER_CONTROLLER_ADS1115_HPP
