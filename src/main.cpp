#include <thread>
#include "ACL/I2C/Device.hpp"
#include "ACL/CAN/CANSocket.hpp"
#include "ACL/CAN/DummyCANSocket.hpp"
#include "ACL/CAN/Message.hpp"
#include "Sensors/ADS1115.hpp"
#include "Processing/BrakeInputThread.hpp"
#include "Utils/Logging/StandardLogger.hpp"
#include "API/Controller/ConfigurationController.hpp"
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

int main(int argc, char *argv[])
{
    bool dummyMode = argc > 1 && std::string(argv[1]) == "--dummy";

    auto logger = new StandardLogger();
    auto configRepository = new ConfigRepository();
    auto userConfig = configRepository->loadUserSettings();

    auto device = new Device(1, 0x48);
    auto sensor = new ADS1115(device);
    auto brakeThread = new BrakeInputThread(sensor, logger);

    MoviDriveService* moviDriveService = nullptr;

    if (dummyMode) {
        std::cout << "Running in CAN dummy mode!\n";
        auto canSocket = new DummyCANSocket();
        moviDriveService = new MoviDriveService(canSocket, logger);
    } else {
        auto canSocket = new CANSocket();
        canSocket->connect("10.0.0.1", 29536);
        canSocket->open();
        moviDriveService = new MoviDriveService(canSocket, logger);
    }

    auto processingService = new ProcessingService(moviDriveService, userConfig);
    auto processingThread = new ProcessingThread(processingService);

    auto configController = new ConfigurationController(processingThread, configRepository);
    auto operationsController = new OperationsController(processingThread);
    auto router = new RequestRouter(operationsController, configController);
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

    std::thread t3([processingThread, brakeThread, webSocketThread] {
        processingThread->start(brakeThread, webSocketThread);
    });
    std::cout << "Processing thread started\n";

    while (true) {
        usleep(50000);
    }
}