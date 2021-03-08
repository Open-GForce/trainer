#include <catch2/catch.hpp>
#include <fakeit.hpp>
#include "../../src/Networking/ControlStatus.hpp"

using namespace GForce::Networking;

TEST_CASE( "MoviDrive control status tests", "[Networking]" )
{
    // Bit n:      1        |    0
    // Bit 3: Hold control  / Release
    // Bit 2: Release       / Soft brake
    // Bit 1: Release       / Hard brake
    // Bit 0: Holding brake / Release

    SECTION("correct binary value for released status")
    {
        auto status = ControlStatus::release();
        REQUIRE(status->toBinary() == 0x6); // 0110
    }

    SECTION("correct binary value for holding control")
    {
        auto status = ControlStatus::holdingControl();
        REQUIRE(status->toBinary() == 0xe); // 1110
    }

    SECTION("correct binary value for soft brake")
    {
        auto status = ControlStatus::softBrake();
        REQUIRE(status->toBinary() == 0x2); // 0010
    }

    SECTION("correct binary value for hard brake")
    {
        auto status = ControlStatus::hardBrake();
        REQUIRE(status->toBinary() == 0x4); // 0100
    }

    SECTION("correct binary value for holding brake")
    {
        auto status = ControlStatus::holdingBrake();
        REQUIRE(status->toBinary() == 0x7); // 0111
    }

    SECTION("correct binary value for full release")
    {
        auto status = ControlStatus::release();
        REQUIRE(status->toBinary() == 0x6); // 0110
    }
}