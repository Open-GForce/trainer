#include <catch2/catch.hpp>
#include <fakeit.hpp>
#include "../../../src/Processing/Mode/RegularSpiralMode.hpp"

using namespace GForce::Processing::Mode;

TEST_CASE( "RegularSpiralMode tests", "[OperationModes]" )
{
    SECTION("Outer brake used for linear braking")
    {
        auto mode = new RegularSpiralMode();
        auto speed = mode->getTargetSpeed(0.5, 0.1);

        CHECK(speed == 0.4);
    }

    SECTION("No outer brake")
    {
        auto mode = new RegularSpiralMode();
        auto speed = mode->getTargetSpeed(0.3, 0);

        CHECK(speed == 0.3);
    }

    SECTION("No brake input at all")
    {
        auto mode = new RegularSpiralMode();
        auto speed = mode->getTargetSpeed(0, 0);

        CHECK(speed == 0);
    }

    SECTION("More brake on outer brake then inner brake")
    {
        auto mode = new RegularSpiralMode();
        auto speed = mode->getTargetSpeed(0.3, 0.5);

        CHECK(speed == 0);
    }
}