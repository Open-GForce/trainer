#include "../../includes/catch2/catch.hpp"
#include "../../includes/fake_it/single_header/catch/fakeit.hpp"
#include "../../src/Processing/ProcessingService.hpp"
#include "../../src/Processing/Mode/RegularSpiralMode.hpp"

using namespace GForce::Processing;
using namespace GForce::Networking;

TEST_CASE( "ProcessingService tests", "[Processing]" )
{
    fakeit::Mock<MoviDriveService> driveServiceMock;
    fakeit::Fake(Method(driveServiceMock, startNode));
    fakeit::Fake(Method(driveServiceMock, setRotationSpeed));
    fakeit::Fake(Method(driveServiceMock, setControlStatus));
    fakeit::Fake(Method(driveServiceMock, sync));
    MoviDriveService* driveService = &driveServiceMock.get();

    auto settings = new UserSettings(new Range(30, 200), new Range(40, 100), 6.3);
    auto service = new ProcessingService(driveService, settings);

    SECTION("Node started during init")
    {
        service->init();
        fakeit::Verify(Method(driveServiceMock, startNode)).Once();
    }

    SECTION("Not released => soft brake and 0 rotation speed")
    {
        fakeit::When(Method(driveServiceMock, setControlStatus)).AlwaysDo([] (ControlStatus* status) {
            CHECK(status->isSoftBrakeActivated());
        });

        service->setFirstBrakeInput(50);
        service->setSecondBrakeInput(80);
        service->setMaxSpeed(600);
        service->setReleased(false);
        service->run();

        fakeit::Verify(Method(driveServiceMock, sync)).Once();
        fakeit::Verify(Method(driveServiceMock, setRotationSpeed)).Once();
        fakeit::Verify(Method(driveServiceMock, setRotationSpeed).Using(0));
        fakeit::Verify(Method(driveServiceMock, setControlStatus)).Once();
    }

    SECTION("RegularSpiralMode => Correct speed calculation based on brakes")
    {
        service->setOperationMode(new Mode::RegularSpiralMode());

        fakeit::When(Method(driveServiceMock, setControlStatus)).AlwaysDo([] (ControlStatus* status) {
            CHECK(!status->isSoftBrakeActivated());
        });

        fakeit::When(Method(driveServiceMock, setRotationSpeed)).AlwaysDo([] (double speed) {
            CHECK(speed == Approx(274.51));
        });

        service->setDirection(RotationDirection::right);
        service->setSecondBrakeInput(100); // Inner brake => 0.4118
        service->setFirstBrakeInput(60); // Outer brake => 0.3333
        service->setMaxSpeed(3500);
        service->setReleased(true);
        service->run();

        fakeit::Verify(Method(driveServiceMock, sync)).Once();
        fakeit::Verify(Method(driveServiceMock, setRotationSpeed)).Once();
        fakeit::Verify(Method(driveServiceMock, setControlStatus)).Once();
    }

    SECTION("Negative speed and inverted brake order for left direction")
    {
        fakeit::When(Method(driveServiceMock, setControlStatus)).AlwaysDo([] (ControlStatus* status) {
            CHECK(!status->isSoftBrakeActivated());
        });

        fakeit::When(Method(driveServiceMock, setRotationSpeed)).AlwaysDo([] (double speed) {
            CHECK(std::round(speed) == -60);
        });

        service->setDirection(RotationDirection::left);
        service->setSecondBrakeInput(0);   // Outer brake => 0
        service->setFirstBrakeInput(200);  // Inner brake => 1
        service->setMaxSpeed(60);
        service->setReleased(true);
        service->run();

        fakeit::Verify(Method(driveServiceMock, sync)).Once();
        fakeit::Verify(Method(driveServiceMock, setRotationSpeed)).Once();
        fakeit::Verify(Method(driveServiceMock, setControlStatus)).Once();
    }

    SECTION("Response object saved during sync")
    {
        fakeit::When(Method(driveServiceMock, sync)).Return(new BusResponse(new EngineStatus(false, false, false, false, false, false, false, false), 500));

        service->setDirection(RotationDirection::left);
        service->run();

        auto status = service->cloneStatus();
        REQUIRE(status != nullptr);
        CHECK(status->getRotationSpeed() == 500);
    }

    SECTION("Response object saved only if not null")
    {
        fakeit::When(Method(driveServiceMock, sync)).Return(new BusResponse(new EngineStatus(false, false, false, false, false, false, false, false), 500));
        service->setDirection(RotationDirection::left);
        service->run();

        fakeit::When(Method(driveServiceMock, sync)).Return(nullptr);
        service->run();

        auto status = service->cloneStatus();
        REQUIRE(status != nullptr);
        CHECK(status->getRotationSpeed() == 500);
    }

    SECTION("Correct processing status")
    {
        fakeit::When(Method(driveServiceMock, sync)).Return(new BusResponse(new EngineStatus(true, false, false, false, false, false, false, false), 500));

        service->setOperationMode(new RegularSpiralMode());
        service->setDirection(RotationDirection::right);
        service->setSecondBrakeInput(115); // Inner brake => 0.4118
        service->setFirstBrakeInput(55); // Outer brake => 0.3333
        service->setMaxSpeed(3500);
        service->setReleased(true);
        service->run();

        auto status = service->getStatus();

        REQUIRE(status->getEngineStatus() != nullptr);
        CHECK(status->getEngineStatus()->isPowerAmplifierReleased());
        CHECK(status->getRotationSpeed() == 500);
        CHECK(status->getInnerBrakeRawValue() == 115);
        CHECK(status->getOuterBrakeRawValue() == 55);
        CHECK(status->getMaxSpeed() == 3500);
        CHECK(status->getTargetSpeed() == Approx(875.0));
        CHECK(status->getInnerBrakePercentage() == Approx(0.5));
        CHECK(status->getOuterBrakePercentage() == Approx(0.25));
        CHECK(status->getRotationDirection() == RotationDirection::right);
        CHECK(status->getOperationMode() == "regularSpiral");
    }

    SECTION("Rotation speed of processing status is always absolute")
    {
        double rotationSpeed = -500;
        fakeit::When(Method(driveServiceMock, sync)).Return(new BusResponse(new EngineStatus(true, false, false, false, false, false, false, false), rotationSpeed));

        service->setOperationMode(new RegularSpiralMode());
        service->setDirection(RotationDirection::right);
        service->setSecondBrakeInput(115); // Inner brake => 0.4118
        service->setFirstBrakeInput(55); // Outer brake => 0.3333
        service->setMaxSpeed(3500);
        service->setReleased(true);
        service->run();

        auto status = service->getStatus();

        REQUIRE(status->getEngineStatus() != nullptr);
        CHECK(status->getRotationSpeed() == 500);
    }
}