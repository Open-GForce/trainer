#include <zconf.h>
#include <iostream>
#include "ADS1115.hpp"
#include "InvalidArgumentException.hpp"

using namespace GForce::Sensors;

ADS1115::ADS1115(Device *device) : device(device) {}

int ADS1115::read(unsigned channel)
{
    if (channel < 0 || channel > 3) {
        throw InvalidArgumentException("Invalid channel " + std::to_string(channel) + " given (Valid: 0-4)");
    }

    uint16_t config = this->compileConfig(channel);
    this->writeConfig(config);
    usleep(10000);

    auto data = device->read(0, 2);
    return (data[0] << 8) | data[1];
}

uint16_t ADS1115::compileConfig(unsigned channel)
{
    uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE     | // Disable the comparator (default val)
                      ADS1015_REG_CONFIG_CLAT_NONLAT   | // Non-latching (default val)
                      ADS1015_REG_CONFIG_CPOL_ACTVLOW  | // Alert/Rdy active low   (default val)
                      ADS1015_REG_CONFIG_CMODE_TRAD    | // Traditional comparator (default val)
                      ADS1015_REG_CONFIG_DR_128SPS     | // 1600 samples per second (default)
                      ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

    // Set PGA/voltage range
    config |= GAIN_TWOTHIRDS;

    // Set single-ended input channel
    switch (channel)
    {
        case (0):
            config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
            break;
        case (1):
            config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
            break;
        case (2):
            config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
            break;
        case (3):
            config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
            break;
        default:
            return 0;
    }

    // Set 'start single-conversion' bit
    config |= ADS1015_REG_CONFIG_OS_SINGLE;

    return config;
}

void ADS1115::writeConfig(uint16_t config)
{
    std::vector<uint8_t> data = {};
    data.push_back((uint8_t)(config >> 8));
    data.push_back((uint8_t)(config & 0xFF));

    this->device->write(ADS1015_REG_POINTER_CONFIG, data);
}

