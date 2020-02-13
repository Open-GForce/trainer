cmake_minimum_required(VERSION 3.7.0)

set(CMAKE_CXX_COMPILER "g++")
project (gforce_trainer_controller LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_BUILD_TYPE Debug)

include_directories(../includes/can-utils)
include_directories(../includes/websocketpp)
include_directories(../includes/nlohmann_json)
include_directories(../includes/spdlog)
include_directories(../includes/rang)

set(SOURCES
        src/ACL/I2C/Device.cpp
        src/ACL/I2C/IOException.cpp
        src/Sensors/ADS1115.cpp
        src/Utils/Exceptions/InvalidArgumentException.cpp
        src/Utils/Exceptions/RuntimeException.cpp
        src/ACL/CAN/Message.cpp
        src/ACL/CAN/CANSocket.cpp
        src/Networking/MoviDriveService.cpp
        src/Networking/ControlStatus.cpp
        src/Utils/Logging/StandardLogger.cpp
        src/ACL/CAN/MessageFactory.cpp
        src/Networking/EngineStatus.cpp
        src/Networking/BusResponse.cpp
        src/Processing/BrakeInput/BrakeInputReceiveThread.cpp
        src/Processing/Range.cpp
        src/Processing/ProcessingThread.cpp
        src/Configuration/UserSettings.cpp
        src/API/Websocket/Server.cpp
        src/Configuration/ConfigRepository.cpp
        src/Processing/ProcessingStatus.cpp
        src/API/Websocket/Response.cpp
        src/API/Websocket/ServerThread.cpp
        src/Processing/ProcessingService.cpp
        src/ACL/CAN/DummyCANSocket.cpp
        src/API/Controller/ConfigurationController.cpp
        src/API/Controller/OperationsController.cpp
        src/Utils/Assertions/Assertion.cpp
        src/Utils/Assertions/AssertionFailedException.cpp
        src/API/Websocket/Request.cpp
        src/API/Websocket/RequestRouter.cpp
        src/Processing/BrakeInput/BrakeInputMessage.cpp
        src/ACL/TCP/BoostTCPSocket.cpp
        src/ACL/TCP/BoostTCPConnection.cpp
        src/Processing/BrakeInput/BrakeInputTransmissionThread.cpp
        src/Processing/Mode/RegularSpiralMode.cpp
        src/Processing/Mode/StaticMaxSpeedMode.cpp
        src/Utils/Algorithms.cpp src/ACL/TCP/BrokenPipeException.hpp)

set(TESTS
        tests/Sensors/ADS1115Test.cpp
        tests/ACL/CAN/MessageTest.cpp
        tests/Networking/ControlStatusTest.cpp
        tests/Networking/MoviDriveServiceTest.cpp
        tests/ACL/CAN/MessageFactoryTest.cpp
        tests/Networking/ResponseTest.cpp
        tests/Processing/BrakeInput/BrakeInputThreadReceiveTest.cpp
        tests/Processing/RangeTest.cpp
        tests/Processing/ProcessingServiceTest.cpp
        tests/Processing/ProcessingThreadTest.cpp
        tests/Configuration/ConfigRepositoryIntegrationTest.cpp
        tests/Processing/ProcessingStatusTest.cpp
        tests/API/Websocket/ServerThreadTest.cpp
        tests/API/Controller/OperationsControllerTest.cpp
        tests/API/Websocket/RequestRouterTest.cpp
        tests/API/Controller/ConfigurationControllerTest.cpp
        tests/Processing/BrakeInput/BrakeInputMessageTest.cpp
        tests/Processing/BrakeInput/BrakeInputTransmissionThreadTest.cpp
        tests/Processing/Mode/RegularSpiralModeTest.cpp
        tests/Processing/Mode/StaticMaxSpeedModeTest.cpp
        tests/General/AlgorithmsTest.cpp)

FIND_PACKAGE( Boost 1.62 COMPONENTS system filesystem REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(controller ${SOURCES} src/main.cpp)
target_link_libraries (controller ${PROJECT_SOURCE_DIR}/includes/can-utils/build/libcan.a)
target_link_libraries (controller pthread)
target_link_libraries (controller m)
target_link_libraries (controller ${Boost_LIBRARIES} )

add_executable(run_tests ${SOURCES} ${TESTS} tests/main.cpp)
target_link_libraries (run_tests ${PROJECT_SOURCE_DIR}/includes/can-utils/build/libcan.a)
target_link_libraries (run_tests pthread)
target_link_libraries (run_tests m)
target_link_libraries (run_tests ${Boost_LIBRARIES} )