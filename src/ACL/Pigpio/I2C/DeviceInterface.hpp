#ifndef GForce_GLIDER_SENSOR_DEVICE_I2C_INTERFACE_H
#define GForce_GLIDER_SENSOR_DEVICE_I2C_INTERFACE_H

namespace GForce {
namespace ACL {
namespace Pigpio {
namespace I2C {

class DeviceInterface
{
    public:
        virtual void open() = 0;
        virtual void close() = 0;

        virtual int readByteData(unsigned registerAddress) = 0;
        virtual int writeByteData(unsigned registerAddress, unsigned data) = 0;

        virtual bool isOpen() = 0;
        virtual unsigned getBus() = 0;
        virtual unsigned getAddress() = 0;
        virtual unsigned getFlags() = 0;
};

}}}}


#endif //GForce_GLIDER_SENSOR_DEVICE_I2C_INTERFACE_H
