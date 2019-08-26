#include "ADS1115.hpp"

using namespace GForce::Sensors;

ADS1115::ADS1115(Device *device) : device(device) {}

double ADS1115::read(unsigned channel)
{
    this->device->write(1, {0xc3, 0x85});
    std::vector<uint8_t> data;
    do {
        data = device->read(1, 2);
    } while (!(data[0] & 0x80));

    data = device->read(0, 2);
    int val = (int16_t) data[0] * 256 + (uint16_t) data[1];

    return val * 4.096 / 32768.0;
}


