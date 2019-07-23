#include <iostream>
#include <zconf.h>
#include "ACL/I2C/Device.hpp"

extern  "C"
{
#include <pigpio.h>
}

using namespace GForce::ACL::I2C;

int main() 
{
    auto device = new Device(1, 0x48);

    device->open();

    while (true) {
        device->write(1, {0xc3, 0x85});
        std::vector<uint8_t> data;
        do {
            data = device->read(1, 2);
        } while (!(data[0] & 0x80));

        data = device->read(0, 2);
        int val = (int16_t) data[0] * 256 + (uint16_t) data[1];

        printf("Hex: 0x%02x%02x - Int: %d - Float, converted: %f V\n", data[0], data[1], val, (float)val*4.096/32768.0);
        usleep(500000);
    }
}