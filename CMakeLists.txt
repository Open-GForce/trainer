cmake_minimum_required(VERSION 3.7.0)

set(CMAKE_CXX_COMPILER "g++")
project (gforce_trainer_controller LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 17)

set(SOURCES
        src/ACL/I2C/Device.cpp
        src/ACL/I2C/IOException.cpp
        src/Sensors/ADS1115.cpp)

set(TESTS)

set_source_files_properties(../nlohmann_json/json.hpp PROPERTIES COMPILE_FLAGS -Wno-effc++)

add_executable(controller ${SOURCES} src/main.cpp)
target_link_libraries (controller pthread)
target_link_libraries (controller m)
target_link_libraries (controller tacopie)


add_executable(run_tests ${SOURCES} ${TESTS} tests/main.cpp)
target_link_libraries (run_tests pthread)
target_link_libraries (run_tests m)
target_link_libraries (run_tests tacopie)