cmake_minimum_required(VERSION 3.7.0)

set(CMAKE_CXX_COMPILER "g++")
project (gforce_trainer_controller LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_BUILD_TYPE Debug)
set (Boost_USE_STATIC_LIBS ON)

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
        src/Processing/BrakeInput/IP/BrakeInputRXThread.cpp
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
        src/Processing/BrakeInput/IP/BrakeInputMessage.cpp
        src/ACL/TCP/BoostTCPSocket.cpp
        src/ACL/TCP/BoostTCPConnection.cpp
        src/Processing/BrakeInput/IP/BrakeInputTXThread.cpp
        src/Processing/Mode/RegularSpiralMode.cpp
        src/Processing/Mode/StaticMaxSpeedMode.cpp
        src/Utils/Algorithms.cpp
        src/Processing/Acceleration/AccelerationStage.cpp
        src/Processing/Acceleration/AccelerationService.cpp
        src/Configuration/SystemSettings.cpp
        src/Networking/CANThread.cpp
        src/Processing/BrakeInput/CAN/WayconBrakeInputThread.cpp)

set(TESTS
        tests/Sensors/ADS1115Test.cpp
        tests/ACL/CAN/MessageTest.cpp
        tests/Networking/ControlStatusTest.cpp
        tests/Networking/MoviDriveServiceTest.cpp
        tests/ACL/CAN/MessageFactoryTest.cpp
        tests/Networking/ResponseTest.cpp
        tests/Processing/BrakeInput/IP/BrakeInputRXThreadTest.cpp
        tests/Processing/RangeTest.cpp
        tests/Processing/ProcessingServiceTest.cpp
        tests/Processing/ProcessingThreadTest.cpp
        tests/Configuration/ConfigRepositoryIntegrationTest.cpp
        tests/Processing/ProcessingStatusTest.cpp
        tests/API/Websocket/ServerThreadTest.cpp
        tests/API/Controller/OperationsControllerTest.cpp
        tests/API/Websocket/RequestRouterTest.cpp
        tests/API/Controller/ConfigurationControllerTest.cpp
        tests/Processing/BrakeInput/IP/BrakeInputMessageTest.cpp
        tests/Processing/BrakeInput/IP/BrakeInputTXThreadTest.cpp
        tests/Processing/Mode/RegularSpiralModeTest.cpp
        tests/Processing/Mode/StaticMaxSpeedModeTest.cpp
        tests/General/AlgorithmsTest.cpp
        tests/Processing/Acceleration/AccelerationServiceTest.cpp
        tests/Networking/CANThreadTest.cpp
        tests/Processing/BrakeInput/CAN/WayconBrakeInputThreadTest.cpp)

include(build/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(controller ${SOURCES} src/main.cpp)
target_link_libraries (controller pthread)
target_link_libraries (controller m)
target_link_libraries (controller ${CONAN_LIBS})

add_executable(run_tests ${SOURCES} ${TESTS} tests/main.cpp)
target_link_libraries (run_tests pthread)
target_link_libraries (run_tests m)
target_link_libraries (run_tests ${CONAN_LIBS})