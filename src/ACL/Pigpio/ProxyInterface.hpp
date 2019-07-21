#ifndef ACL_PIGPIO_PROXY_INTERFACE_H
#define ACL_PIGPIO_PROXY_INTERFACE_H

namespace GForce {
namespace ACL {
namespace Pigpio {

    class ProxyInterface
    {
        public:            
            virtual int initialize() = 0;
            
            virtual int i2cOpen(unsigned i2cBus, unsigned i2cAddr, unsigned i2cFlags) = 0;
            virtual int i2cClose(unsigned handle) = 0;
            virtual int i2cReadByteData(unsigned handle, unsigned reg) = 0;
            virtual int i2cWriteByteData(unsigned handle, unsigned reg, unsigned bVal) = 0;

            virtual int spiOpen(unsigned spiChan, unsigned baud, unsigned spiFlags) = 0;
            virtual int spiClose(unsigned handle) = 0;
            virtual int spiXfer(unsigned handle, char *txBuf, char *rxBuf, unsigned count) = 0;

            virtual int gpioTime(unsigned timetype, int *seconds, int *micros) = 0;

            virtual bool isInitialized() = 0;
            virtual int getLibraryVersion() = 0;
    };

}}}

#endif