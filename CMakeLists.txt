cmake_minimum_required(VERSION 3.7.0)

set(CMAKE_CXX_COMPILER "g++")
project (gforce_trainer_controller LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_BUILD_TYPE Debug)

include_directories(../can-utils)
include_directories(../websocketpp)
include_directories(../nlohmann_json)

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
        src/Processing/BrakeInputThread.cpp
        src/Processing/Range.cpp
        src/Processing/ProcessingThread.cpp
        src/Configuration/UserSettings.cpp
        src/API/Websocket/Server.cpp
        src/Configuration/ConfigRepository.cpp
        src/Processing/ProcessingStatus.cpp
        src/API/Websocket/Response.cpp
        src/API/Websocket/ServerThread.cpp
        src/Processing/ProcessingService.cpp)

set(TESTS
        tests/Sensors/ADS1115Test.cpp
        tests/ACL/CAN/MessageTest.cpp
        tests/Networking/ControlStatusTest.cpp
        tests/Networking/MoviDriveServiceTest.cpp
        tests/ACL/CAN/MessageFactoryTest.cpp
        tests/Networking/ResponseTest.cpp
        tests/Processing/BrakeInputThreadTest.cpp
        tests/Processing/RangeTest.cpp
        tests/Processing/ProcessingServiceTest.cpp
        tests/Processing/ProcessingThreadTest.cpp
        tests/Configuration/ConfigRepositoryIntegrationTest.cpp
        tests/Processing/ProcessingStatusTest.cpp
        tests/API/Websocket/ServerThreadTest.cpp)

set_source_files_properties(../nlohmann_json/json.hpp PROPERTIES COMPILE_FLAGS -Wno-effc++)

FIND_PACKAGE( Boost 1.62 COMPONENTS system filesystem REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(controller ${SOURCES} src/main.cpp src/API/Websocket/Server.cpp src/API/Websocket/Server.hpp src/Processing/ProcessingStatus.cpp src/Processing/ProcessingStatus.hpp)
target_link_libraries (controller ${PROJECT_SOURCE_DIR}/../can-utils/build/libcan.a)
target_link_libraries (controller pthread)
target_link_libraries (controller m)
target_link_libraries (controller tacopie)
target_link_libraries (controller ${Boost_LIBRARIES} )

add_executable(run_tests ${SOURCES} ${TESTS} tests/main.cpp)
target_link_libraries (run_tests ${PROJECT_SOURCE_DIR}/../can-utils/build/libcan.a)
target_link_libraries (run_tests pthread)
target_link_libraries (run_tests m)
target_link_libraries (run_tests tacopie)
target_link_libraries (run_tests ${Boost_LIBRARIES} )