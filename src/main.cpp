#include <thread>
#include "ACL/I2C/Device.hpp"
#include "Sensors/ADS1115.hpp"
#include "ACL/CAN/CANSocket.hpp"
#include "ACL/CAN/DummyCANSocket.hpp"
#include "Processing/BrakeInput/IP/BrakeInputRXThread.hpp"
#include "Processing/BrakeInput/IP/BrakeInputTXThread.hpp"
#include "Processing/BrakeInput/CAN/WayconBrakeInputThread.hpp"
#include "Utils/Logging/StandardLogger.hpp"
#include "Utils/SystemRepository.hpp"
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
using namespace GForce::Utils;
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
    logger->setGlobalContext("mode", "main_controller");
    logger->info(LOG_CHANNEL_MAIN, "Running main controller mode [" + canMode + " CAN]", {});

    auto configRepository = new ConfigRepository();
    auto userConfig = configRepository->loadUserSettings("default");
    auto systemConfig = configRepository->loadSystemSettings();

    CANThread* canThread = nullptr;
    MoviDriveService* moviDriveService = nullptr;

    if (CANDummyMode) {
        auto canSocket = new DummyCANSocket();
        canThread = new CANThread(canSocket, logger);
        moviDriveService = new MoviDriveService(canThread, logger);
    } else {
        auto canSocket = new CANSocket();
        std::string canSocketAddress = SystemRepository::getNetworkAddress("eth0");

        logger->info(LOG_CHANNEL_MAIN, "Connecting to CAN socket daemon on " + canSocketAddress + ":29536", {});

        canSocket->connect(canSocketAddress, 29536);
        canSocket->open();
        canThread = new CANThread(canSocket, logger);
        moviDriveService = new MoviDriveService(canThread, logger);
    }

    auto accelerationService = new AccelerationService();
    auto processingService = new ProcessingService(moviDriveService, userConfig, accelerationService);
    auto processingThread = new ProcessingThread(logger, processingService);

    BrakeInputThread* brakeThread = nullptr;
    if (systemConfig->getBrakeSensorProtocol() == BrakeSensorProtocol::IPNetwork) {
        brakeThread = new IP::BrakeInputRXThread(logger);
    } else {
        brakeThread = new BrakeInput::CAN::WayconBrakeInputThread(canThread, logger);
    }

    auto configController = new ConfigurationController(processingThread, configRepository, logger);
    auto operationsController = new OperationsController(processingThread);
    auto router = new RequestRouter(operationsController, configController);
    auto websocketServer = new Server(router, logger);
    auto webSocketThread = new ServerThread(websocketServer, logger);

    std::thread t0([websocketServer] {
        websocketServer->run(8763);
    });

    std::thread t1([canThread] {
        canThread->start();
    });
    logger->info(LOG_CHANNEL_MAIN, "CAN thread started", {});

    std::thread t2([webSocketThread] {
        webSocketThread->start();
    });
    logger->info(LOG_CHANNEL_MAIN, "Websocket thread started", {});

    std::thread t3([brakeThread] {
        brakeThread->start();
    });
    brakeThread->waitUntilStarted();
    logger->info(LOG_CHANNEL_MAIN, "Brake input thread started", {});

    std::thread t4([processingThread, brakeThread, webSocketThread] {
        processingThread->start(brakeThread, webSocketThread);
    });
    logger->info(LOG_CHANNEL_MAIN, "Processing thread started", {});

    while (true) {
        usleep(50000);
    }
}

void runBrakeInputMode()
{
    auto logger = new StandardLogger();
    logger->setGlobalContext("mode", "brake_input");
    logger->info(LOG_CHANNEL_MAIN, "Running brake input mode", {});

    auto device = new Device(1, 0x48);
    device->open();

    auto sensor = new ADS1115(device);
    auto thread = new IP::BrakeInputTXThread(logger, sensor);

    thread->start();
}