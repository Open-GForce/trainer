#ifndef GFORCE_TRAINER_CONTROLLER_DEVICE_HPP
#define GFORCE_TRAINER_CONTROLLER_DEVICE_HPP

#include <vector>
#include <cstdint>

namespace GForce::ACL::I2C {

class Device
{
    private:
        unsigned bus;
        unsigned address;

        /* Device _handleMessage */
        int handle;

        unsigned selectedRegister;

        void assertOpen();
    public:
        Device(unsigned int bus, unsigned int address);

        virtual ~Device();

        virtual void open();
        virtual void write(int i2cRegister, const std::vector<uint8_t>& data);
        virtual std::vector<uint8_t> read(int i2cRegister, int count);
};

}


#endif //GFORCE_TRAINER_CONTROLLER_DEVICE_HPP
