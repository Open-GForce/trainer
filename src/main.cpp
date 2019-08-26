#include <iostream>
#include <zconf.h>
#include "ACL/I2C/Device.hpp"
#include "Sensors/ADS1115.hpp"

extern  "C"
{
#include <pigpio.h>
}

using namespace GForce::ACL::I2C;
using namespace GForce::Sensors;


int main() 
{
    auto device = new Device(1, 0x48);
    auto sensor = new ADS1115(device);

    device->open();

    while (true) {
        std::cout << "Voltage: " << sensor->read(0) << " V\n";
        usleep(500000);
    }
}