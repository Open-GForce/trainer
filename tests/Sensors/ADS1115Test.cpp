#include <catch2/catch.hpp>
#include <fakeit.hpp>
#include "../../src/Sensors/ADS1115.hpp"

using namespace GForce::ACL::I2C;
using namespace GForce::Sensors;

TEST_CASE( "ADS1115 test", "[Sensors]" ) {
    fakeit::Mock<Device> deviceMock;
    fakeit::Fake(Method(deviceMock, write));
    fakeit::Fake(Method(deviceMock, read));
    Device *device = &deviceMock.get();

    auto sensor = new ADS1115(device);

    SECTION("Correct default config")
    {
        fakeit::When(Method(deviceMock, read)).AlwaysReturn({0, 0});

        fakeit::When(Method(deviceMock, write)).AlwaysDo([] (int i2cRegister, const std::vector<uint8_t>& data) {
            CHECK(i2cRegister == ADS1015_REG_POINTER_CONFIG);
            CHECK(data.size() == 2);

            CHECK((int) data[0] == 0xC1);
            CHECK((int) data[1] == 0xC3);
        });

        sensor->read(0);
    }

    SECTION("Correct channel in config")
    {
        fakeit::When(Method(deviceMock, read)).AlwaysReturn({0, 0});

        fakeit::When(Method(deviceMock, write)).AlwaysDo([] (int i2cRegister, const std::vector<uint8_t>& data) {
            CHECK(i2cRegister == ADS1015_REG_POINTER_CONFIG);
            CHECK(data.size() == 2);

            uint16_t config = ((int) data[0]) << 8 | ((int) data[1]);
            CHECK((config | ADS1015_REG_CONFIG_MUX_SINGLE_2) == config);
        });

        sensor->read(2);
    }

    SECTION("Correct value calculation")
    {
        fakeit::When(Method(deviceMock, read)).AlwaysReturn({150, 240});

        fakeit::When(Method(deviceMock, write)).AlwaysDo([] (int i2cRegister, const std::vector<uint8_t>& data) {
            CHECK(i2cRegister == ADS1015_REG_POINTER_CONFIG);
            CHECK(data.size() == 2);

            uint16_t config = ((int) data[0]) << 8 | ((int) data[1]);
            CHECK((config | ADS1015_REG_CONFIG_MUX_SINGLE_2) == config);
        });

        CHECK(sensor->read(2) == 38640);
    }
}