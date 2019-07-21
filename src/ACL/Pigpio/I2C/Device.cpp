#include "Device.hpp"
#include "../PigpioException.hpp"
#include "../LogicException.hpp"

extern  "C"
{
#include <pigpio.h>
}


using namespace GForce::ACL::Pigpio;

I2C::Device::Device(unsigned bus, unsigned address, unsigned flags, ProxyInterface *proxy)
{
    this->bus = bus;
    this->address = address;
    this->flags = flags;
    this->proxy = proxy;
    this->handle = -1;
}

void I2C::Device::open()
{
    if (this->handle >= 0) {
        return;
    }

    int result = proxy->i2cOpen(this->bus, this->address, this->flags);

    if (result >= 0) {
        this->handle = result;
        return;
    }

    switch (result) {
        case PI_BAD_I2C_BUS:
            throw PigpioException("Opening i2c device failed => invalid bus given");
        case PI_BAD_I2C_ADDR:
            throw PigpioException("Opening i2c device failed => invalid address given");
        case PI_BAD_FLAGS:
            throw PigpioException("Opening i2c device failed => invalid flags given");
        case PI_NO_HANDLE:
            throw PigpioException("Opening i2c device failed => no handle available");
        case PI_I2C_OPEN_FAILED:
            throw PigpioException("Opening i2c device failed => internal device error");
        default:
            throw PigpioException("Opening i2c device failed => unknown error (RC:" + std::to_string(result) + ")");
    }
}

void I2C::Device::close()
{
    if (!this->isOpen()) {
        return;
    }

    int result = proxy->i2cClose((unsigned) this->handle);

    if (result != 0) {
        throw PigpioException("Closing i2c device failed => unknown error (RC:" + std::to_string(result) + ")");
    }

    this->handle = -1;
}

int I2C::Device::readByteData(unsigned registerAddress)
{
    this->assertDeviceOpen();
    int result = this->proxy->i2cReadByteData((unsigned) this->handle, registerAddress);
    this->checkForTransferError("byte read", result);

    return result;
}

int I2C::Device::writeByteData(unsigned registerAddress, unsigned data)
{
    this->assertDeviceOpen();
    int result = this->proxy->i2cWriteByteData((unsigned) this->handle, registerAddress, data);
    this->checkForTransferError("byte write", result);

    return result;
}

void I2C::Device::checkForTransferError(std::string transferType, int returnValue)
{
    if (returnValue >= 0) {
        return;
    }

    std::string baseMessage = "Data transfer (" + transferType + ") with i2c device failed => ";

    switch (returnValue) {
        case PI_BAD_HANDLE:
            throw PigpioException(baseMessage + "invalid handle used");
        case PI_BAD_PARAM:
            throw PigpioException(baseMessage + "invalid parameter (register)");
        case PI_I2C_READ_FAILED:
        case PI_I2C_WRITE_FAILED:
            throw PigpioException(baseMessage + "internal device error");
        default:
            throw PigpioException(baseMessage + "unknown error (RC:" + std::to_string(returnValue) + ")");
    }
}

void I2C::Device::assertDeviceOpen()
{
    if (!this->isOpen()) {
        throw LogicException("Unable to interact with non-open i2c device");
    }
}

bool I2C::Device::isOpen()
{
    return this->handle >= 0;
}

unsigned I2C::Device::getAddress()
{
    return this->address;
}

unsigned I2C::Device::getBus()
{
    return this->bus;
}

unsigned I2C::Device::getFlags()
{
    return this->flags;
}