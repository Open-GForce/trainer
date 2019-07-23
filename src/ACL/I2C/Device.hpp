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

        /* Device handle */
        int handle;

        unsigned selectedRegister;

        void assertOpen();
    public:
        Device(unsigned int bus, unsigned int address);

        virtual ~Device();

        void open();
        void write(int i2cRegister, const std::vector<uint8_t>& data);
        std::vector<uint8_t> read(int i2cRegister, int count);
};

}


#endif //GFORCE_TRAINER_CONTROLLER_DEVICE_HPP
