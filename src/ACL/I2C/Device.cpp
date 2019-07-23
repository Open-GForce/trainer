#include "Device.hpp"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <linux/i2c-dev.h>
#include <stropts.h>
#include <stdexcept>
#include <iostream>
#include "IOException.hpp"

using namespace GForce::ACL::I2C;

Device::Device(unsigned int bus, unsigned int address) : bus(bus), address(address)
{
    this->handle = -1;
    this->selectedRegister = -1;
}

Device::~Device()
{
    close(this->handle);
}

void Device::open()
{
    if ((this->handle = ::open("/dev/i2c-1", O_RDWR)) < 0) {
        throw std::runtime_error{"Unable to open i2c device"};
    }

    if (ioctl(this->handle, I2C_SLAVE, this->address) < 0) {
        throw std::runtime_error{"Error: Couldn't find i2c device on address" + std::to_string(this->address) + "!"};
    }
}

void Device::write(int i2cRegister, const std::vector<uint8_t>& data)
{
    this->assertOpen();
    this->selectedRegister = i2cRegister;

    int byteCount = data.size() + 1;

    uint8_t buffer[byteCount];
    buffer[0] = i2cRegister;

    int index = 1;
    for (auto &item : data) {
        buffer[index++] = item;
    }

    int bytesWritten = ::write(this->handle, buffer, byteCount);
    if (bytesWritten != byteCount) {
        throw IOException("Failure while writing data to i2c device");
    }
}

std::vector<uint8_t> Device::read(int i2cRegister, int count)
{
    this->assertOpen();

    uint8_t buffer[count];
    if (this->selectedRegister != i2cRegister) {
        this->write(i2cRegister, {});
    }

    int bytesRead = ::read(this->handle, buffer, count);
    if (bytesRead != count) {
        throw IOException("Failure while reading data from i2c device");
    }

    return std::vector<uint8_t>(buffer, buffer + sizeof buffer / sizeof buffer[0]);
}

void Device::assertOpen()
{
    if (this->handle == -1) {
        throw std::logic_error{"i2c device needs to be opened for transferring data"};
    }
}
