#include <thread>
#include "ACL/I2C/Device.hpp"
#include "Sensors/ADS1115.hpp"
#include "ACL/CAN/CANSocket.hpp"
#include "ACL/CAN/DummyCANSocket.hpp"
#include "Processing/BrakeInput/BrakeInputReceiveThread.hpp"
#include "Processing/BrakeInput/BrakeInputTransmissionThread.hpp"
#include "Utils/Logging/StandardLogger.hpp"
#include "API/Controller/ConfigurationController.hpp"
#include "API/Controller/OperationsController.hpp"
#include "API/Websocket/RequestRouter.hpp"
#include "API/Websocket/ServerThread.hpp"
#include "Processing/ProcessingService.hpp"
#include "Processing/ProcessingThread.hpp"
#include "Configuration/ConfigRepository.hpp"

using namespace GForce::ACL::I2C;
using namespace GForce::ACL::CAN;
using namespace GForce::API::Websocket;
using namespace GForce::API::Controller;
using namespace GForce::Processing;
using namespace GForce::Sensors;
using namespace GForce::Processing::BrakeInput;
using namespace GForce::Utils::Logging;

void runControllerMode(bool CANDummyMode);
void runBrakeInputMode();

int main(int argc, char *argv[])
{
    bool dummyMode = argc > 1 && std::string(argv[1]) == "--dummy";
    bool brakeInputMode = argc > 1 && std::string(argv[1]) == "--brake_input";

    if (brakeInputMode) {
        runBrakeInputMode();
    } else {
        runControllerMode(dummyMode);
    }
}

void runControllerMode(bool CANDummyMode)
{
    auto logger = new StandardLogger();
    std::string canMode = (CANDummyMode ? "Dummy" : "Regular");
    logger->info("Running main controller mode [" + canMode + " CAN]");

    auto configRepository = new ConfigRepository();
    auto userConfig = configRepository->loadUserSettings();

    MoviDriveService* moviDriveService = nullptr;

    if (CANDummyMode) {
        auto canSocket = new DummyCANSocket();
        moviDriveService = new MoviDriveService(canSocket, logger);
    } else {
        auto canSocket = new CANSocket();
        canSocket->connect("192.168.2.201", 29536);
        canSocket->open();
        moviDriveService = new MoviDriveService(canSocket, logger);
    }

    auto processingService = new ProcessingService(moviDriveService, userConfig);
    auto processingThread = new ProcessingThread(logger, processingService);

    auto brakeThread = new BrakeInputReceiveThread(logger);

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
    logger->info("Websocket thread started");

    std::thread t2([brakeThread] {
        brakeThread->start();
    });
    brakeThread->waitUntilStarted();
    logger->info("Brake input thread started");

    std::thread t3([processingThread, brakeThread, webSocketThread] {
        processingThread->start(brakeThread, webSocketThread);
    });
    logger->info("Processing thread started");

    while (true) {
        usleep(50000);
    }
}

void runBrakeInputMode()
{
    auto logger = new StandardLogger();
    logger->info("Running brake input mode");

    auto device = new Device(1, 0x48);
    device->open();

    auto sensor = new ADS1115(device);
    auto thread = new BrakeInputTransmissionThread(logger, sensor);

    thread->start();
}