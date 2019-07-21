#include "ProxyInterface.hpp"

#ifndef ACL_PIGPIO_PROXY_H
#define ACL_PIGPIO_PROXY_H

namespace GForce {
namespace ACL {
namespace Pigpio {

    class Proxy : public ProxyInterface
    {
        const unsigned PI_TIME_RELATIVE = PI_TIME_RELATIVE;

        private:
            bool initialized;
            int libraryVersion;

        public:
            Proxy();
            
            int initialize();
            
            int i2cOpen(unsigned i2cBus, unsigned i2cAddr, unsigned i2cFlags);
            int i2cClose(unsigned handle);
            int i2cReadByteData(unsigned handle, unsigned reg);
            int i2cWriteByteData(unsigned handle, unsigned reg, unsigned bVal);

            int spiOpen(unsigned spiChan, unsigned baud, unsigned spiFlags);
            int spiClose(unsigned handle);
            int spiXfer(unsigned handle, char *txBuf, char *rxBuf, unsigned count);

            int gpioTime(unsigned timetype, int *seconds, int *micros);

            bool isInitialized();
            int getLibraryVersion();
    };

}
}
}

#endif