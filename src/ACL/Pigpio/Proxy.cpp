#include "Proxy.hpp"
#include "InitFailureException.hpp"

extern  "C"
{
#include <pigpio.h>
}

using namespace GForce::ACL::Pigpio;

Proxy::Proxy()
{
    this->initialized = false;
    this->libraryVersion = -1;
}

int Proxy::initialize()
{
    if (this->initialized) {
        return this->libraryVersion;
    }

    int result = gpioInitialise();

    if (result == PI_INIT_FAILED) {
        throw InitFailureException("Initializing library Pigpio failed (PI_INIT_FAILED)");
    }

    this->initialized = true;
    this->libraryVersion = result;
    return result;
}

int Proxy::i2cOpen(unsigned i2cBus, unsigned i2cAddr, unsigned i2cFlags) 
{
    return ::i2cOpen(i2cBus, i2cAddr, i2cFlags);
}

int Proxy::i2cClose(unsigned handle) 
{
    return ::i2cClose(handle);
}

int Proxy::i2cReadByteData(unsigned handle, unsigned reg) 
{
    return ::i2cReadByteData(handle, reg);
}

int Proxy::i2cWriteByteData(unsigned handle, unsigned reg, unsigned bVal)
{
    return ::i2cWriteByteData(handle, reg, bVal);
}

int Proxy::spiOpen(unsigned spiChan, unsigned baud, unsigned spiFlags)
{
    return ::spiOpen(spiChan, baud, spiFlags);
}

int Proxy::spiClose(unsigned handle)
{
    return ::spiClose(handle);
}

int Proxy::spiXfer(unsigned handle, char *txBuf, char *rxBuf, unsigned count)
{
    return ::spiXfer(handle, txBuf, rxBuf, count);
}

int Proxy::gpioTime(unsigned timetype, int *seconds, int *micros)
{
    return ::gpioTime(timetype, seconds, micros);
}

bool Proxy::isInitialized()
{
    return this->initialized;
}

int Proxy::getLibraryVersion()
{
    return this->libraryVersion;
}