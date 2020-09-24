#include "../../../includes/catch2/catch.hpp"
#include "../../../src/Processing/Acceleration/AccelerationService.hpp"
#include "../../../src/Processing/Acceleration/AccelerationStage.hpp"

using namespace GForce::Processing::Acceleration;

TEST_CASE( "AccelerationService targetSpeed mode tests", "[Processing Acceleration]" )
{
    auto service = new AccelerationService();
    service->setAccelerationMode(AccelerationMode::targetSpeed);

    SECTION("Within soft-start")
    {
        service->setSoftStartAcceleration(1000);
        service->setSoftStartSpeed(10);

        service->setStages({
            AccelerationStage(0, 5000)
        });

        int result = service->getAcceleration(5, 50);
        CHECK(result == 1000);
    }

    SECTION("Minimal speed")
    {
        service->setSoftStartAcceleration(1000);
        service->setSoftStartSpeed(10);

        service->setStages({
               AccelerationStage(20, 3000),
               AccelerationStage(0, 2000),
           });

        int result = service->getAcceleration(11, 15);
        CHECK(result == 2000);
    }

    SECTION("Maximum speed")
    {
        service->setSoftStartAcceleration(1000);
        service->setSoftStartSpeed(10);

        service->setStages({
           AccelerationStage(20, 3000),
           AccelerationStage(0, 2000),
        });

        int result = service->getAcceleration(11, 21);
        CHECK(result == 3000);
    }

    SECTION("Intermediate speed")
    {
        service->setSoftStartAcceleration(1000);
        service->setSoftStartSpeed(10);

        service->setStages({
           AccelerationStage(30, 4000),
           AccelerationStage(20, 3500),
           AccelerationStage(0, 2000),
        });

        int result = service->getAcceleration(11, 25);
        CHECK(result == 3500);
    }

    SECTION("Incomplete list => soft acceleration used")
    {
        service->setSoftStartAcceleration(1000);
        service->setSoftStartSpeed(10);

        service->setStages({
           AccelerationStage(30, 4000),
        });

        int result = service->getAcceleration(11, 25);
        CHECK(result == 1000);
    }

    SECTION("Empty list => soft acceleration used")
    {
        service->setSoftStartAcceleration(1000);
        service->setSoftStartSpeed(10);

        service->setStages({});

        int result = service->getAcceleration(11, 25);
        CHECK(result == 1000);
    }
}


TEST_CASE( "AccelerationService differential mode tests", "[Processing Acceleration]" )
{
    auto service = new AccelerationService();
    service->setAccelerationMode(AccelerationMode::differential);

    SECTION("Within soft-start")
    {
        service->setSoftStartAcceleration(1000);
        service->setSoftStartSpeed(10);

        service->setStages({
           AccelerationStage(0, 5000)
        });

        int result = service->getAcceleration(5, 50);
        CHECK(result == 1000);
    }

    SECTION("Minimal speed")
    {
        service->setSoftStartAcceleration(1000);
        service->setSoftStartSpeed(10);

        service->setStages({
            AccelerationStage(20, 3000),
            AccelerationStage(0, 2000),
        });

        int result = service->getAcceleration(11, 21);
        CHECK(result == 2000);
    }

    SECTION("Maximum speed")
    {
        service->setSoftStartAcceleration(1000);
        service->setSoftStartSpeed(10);

        service->setStages({
            AccelerationStage(20, 3000),
            AccelerationStage(0, 2000),
       });

        int result = service->getAcceleration(11, 40);
        CHECK(result == 3000);
    }

    SECTION("Intermediate speed")
    {
        service->setSoftStartAcceleration(1000);
        service->setSoftStartSpeed(10);

        service->setStages({
            AccelerationStage(30, 4000),
            AccelerationStage(20, 3500),
            AccelerationStage(0, 2000),
        });

        int result = service->getAcceleration(11, 33);
        CHECK(result == 3500);
    }

    SECTION("Incomplete list => soft acceleration used")
    {
        service->setSoftStartAcceleration(1000);
        service->setSoftStartSpeed(10);

        service->setStages({
            AccelerationStage(30, 4000),
        });

        int result = service->getAcceleration(11, 25);
        CHECK(result == 1000);
    }

    SECTION("Empty list => soft acceleration used")
    {
        service->setSoftStartAcceleration(1000);
        service->setSoftStartSpeed(10);

        service->setStages({});

        int result = service->getAcceleration(11, 25);
        CHECK(result == 1000);
    }
}