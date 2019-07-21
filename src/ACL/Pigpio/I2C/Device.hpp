#include <iostream>
#include "../ProxyInterface.hpp"
#include "DeviceInterface.hpp"

#ifndef GForce_ACL_PIGPIO_I2C_DEVICE_H
#define GForce_ACL_PIGPIO_I2C_DEVICE_H

namespace GForce {
namespace ACL {
namespace Pigpio {
namespace I2C {

    class Device : public DeviceInterface
    {
        private:
            unsigned bus;
            unsigned address;
            unsigned flags;

            ProxyInterface *proxy;
            int handle;

            void assertDeviceOpen();
            void checkForTransferError(std::string transferType, int returnValue);

        public:
            Device(unsigned bus, unsigned address, unsigned flags, ProxyInterface *proxy);
            
            void open();
            void close();
            
            int readByteData(unsigned registerAddress);
            int writeByteData(unsigned registerAddress, unsigned data);

            bool isOpen();
            unsigned getBus();
            unsigned getAddress();
            unsigned getFlags();
    };

}}}}

#endif