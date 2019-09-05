#include <thread>
#include "ACL/I2C/Device.hpp"
#include "ACL/CAN/CANSocket.hpp"
#include "ACL/CAN/DummyCANSocket.hpp"
#include "ACL/CAN/Message.hpp"
#include "Sensors/ADS1115.hpp"
#include "Processing/BrakeInputThread.hpp"
#include "Utils/Logging/StandardLogger.hpp"
#include "API/Controller/OperationsController.hpp"
#include "API/Websocket/RequestRouter.hpp"
#include "API/Websocket/ServerThread.hpp"
#include "Processing/ProcessingService.hpp"
#include "Processing/ProcessingThread.hpp"
#include "Configuration/ConfigRepository.hpp"

extern  "C"
{
#include <pigpio.h>
}

using namespace GForce::ACL::I2C;
using namespace GForce::ACL::CAN;
using namespace GForce::API::Websocket;
using namespace GForce::API::Controller;
using namespace GForce::Sensors;
using namespace GForce::Processing;
using namespace GForce::Utils::Logging;

int main() 
{
    auto logger = new StandardLogger();
    auto configRepository = new ConfigRepository();
    auto userConfig = configRepository->loadUserSettings();

    auto device = new Device(1, 0x48);
    auto sensor = new ADS1115(device);
    auto brakeThread = new BrakeInputThread(sensor, logger);

    auto canSocket = new DummyCANSocket();
    auto moviDriveService = new MoviDriveService(canSocket, logger);
    auto processingService = new ProcessingService(moviDriveService, userConfig);
    auto processingThread = new ProcessingThread(processingService);

    auto operationsController = new OperationsController(processingThread);
    auto router = new RequestRouter(operationsController);
    auto websocketServer = new Server(router, logger);
    auto webSocketThread = new ServerThread(websocketServer, logger);

    std::thread t0([websocketServer] {
        websocketServer->run(8763);
    });

    std::thread t1([webSocketThread] {
        webSocketThread->start();
    });
    std::cout << "Websocket thread started\n";

    device->open();
    std::thread t2([brakeThread] {
        brakeThread->start();
    });
    std::cout << "Brake input thread started\n";

    //canSocket->connect("192.168.2.102", 29536);
    //canSocket->open();
    std::thread t3([processingThread, brakeThread, webSocketThread] {
        processingThread->start(brakeThread, webSocketThread);
    });
    std::cout << "Processing thread started\n";

    while (true) {
        usleep(50000);
    }
}