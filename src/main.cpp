#include <iostream>
#include <zconf.h>
#include "ACL/I2C/Device.hpp"
#include "ACL/CAN/CANSocket.hpp"
#include "Sensors/ADS1115.hpp"

extern  "C"
{
#include <pigpio.h>
}

using namespace GForce::ACL::I2C;
using namespace GForce::ACL::CAN;
using namespace GForce::Sensors;

int main() 
{
    auto device = new Device(1, 0x48);
    auto sensor = new ADS1115(device);

    auto canSocket = new CANSocket();
    canSocket->open();

    canSocket->send(new Message(0x702, {}));
    //
    //device->open();
    //
    //while (true) {
    //    std::cout << "Voltage: " << sensor->read(0) << " V\n";
    //    usleep(500000);
    //}
}