#include <catch2/catch.hpp>
#include <fakeit.hpp>
#include "../../../src/Processing/Mode/StaticMaxSpeedMode.hpp"

using namespace GForce::Processing::Mode;

TEST_CASE( "StaticMaxSpeedMode tests", "[OperationModes]" )
{
    SECTION("Brake inputs ignored, always 100%")
    {
        auto mode = new StaticMaxSpeedMode();
        auto speed = mode->getTargetSpeed(0.5, 0.1);

        CHECK(speed == 1);
    }
}