#include <iostream>
#include <zconf.h>
#include <deque>
#include <thread>
#include "ACL/I2C/Device.hpp"
#include "ACL/CAN/CANSocket.hpp"
#include "ACL/CAN/Message.hpp"
#include "Sensors/ADS1115.hpp"
#include "Processing/BrakeInputThread.hpp"
#include "Utils/Logging/StandardLogger.hpp"

extern  "C"
{
#include <pigpio.h>
}

using namespace GForce::ACL::I2C;
using namespace GForce::ACL::CAN;
using namespace GForce::Sensors;
using namespace GForce::Processing;
using namespace GForce::Utils::Logging;

int main() 
{
    auto device = new Device(1, 0x48);
    auto sensor = new ADS1115(device);
    auto logger = new StandardLogger();

    auto brakeThread = new BrakeInputThread(sensor, logger);

    device->open();

    std::thread t1([brakeThread] {
        brakeThread->start();
    });

    while (true) {
        std::cout << "First brake: " << brakeThread->getFirstBrake() << "\n";
        std::cout << "Second brake: " << brakeThread->getSecondBrake() << "\n\n";
        usleep(50000);
    }
}