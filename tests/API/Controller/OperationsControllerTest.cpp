#include <catch2/catch.hpp>
#include <fakeit.hpp>
#include "../../../src/API/Controller/OperationsController.hpp"
#include "../../../src/API/Controller/HeartbeatResponse.hpp"
#include "../../../src/Utils/Assertions/AssertionFailedException.hpp"

using namespace GForce::API::Controller;
using namespace GForce::Utils::Assertions;

TEST_CASE( "OperationsController tests", "[Controller]" )
{
    fakeit::Mock<ProcessingThread> processingThreadMock;
    fakeit::Fake(Method(processingThreadMock, setMaxSpeed));
    fakeit::Fake(Method(processingThreadMock, setDirection));
    fakeit::Fake(Method(processingThreadMock, setReleased));
    ProcessingThread* processingThread = &processingThreadMock.get();

    auto controller = new OperationsController(processingThread);

    SECTION("handleSpeedLimit() => speed field is missing")
    {
        auto request = new Request("setMaxSpeed", {{}});

        try {
            controller->handleSpeedLimit(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field speed");
        }
    }

    SECTION("handleSpeedLimit() => speed field is not number")
    {
        auto request = new Request("setMaxSpeed", {{"speed", "abc"}});

        try {
            controller->handleSpeedLimit(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field speed is not a number");
        }
    }

    SECTION("handleSpeedLimit() => processing thread called")
    {
        auto request = new Request("setMaxSpeed", {{"speed", 348.31}});
        controller->handleSpeedLimit(request);

        fakeit::Verify(Method(processingThreadMock, setMaxSpeed)).Once();
        fakeit::Verify(Method(processingThreadMock, setMaxSpeed).Using(348.31));
    }

    SECTION("handleRotationDirection() => direction field is missing")
    {
        auto request = new Request("setRotationDirection", {{}});

        try {
            controller->handleRotationDirection(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field direction");
        }
    }

    SECTION("handleRotationDirection() => speed field is not string")
    {
        auto request = new Request("setRotationDirection", {{"direction", 12893}});

        try {
            controller->handleRotationDirection(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field direction is not a string");
        }
    }

    SECTION("handleRotationDirection() => processing thread called => right")
    {
        auto request = new Request("setRotationDirection", {{"direction", "right"}});
        controller->handleRotationDirection(request);

        fakeit::Verify(Method(processingThreadMock, setDirection)).Once();
        fakeit::Verify(Method(processingThreadMock, setDirection).Using(RotationDirection::right));
    }

    SECTION("handleRotationDirection() => processing thread called => left")
    {
        auto request = new Request("setRotationDirection", {{"direction", "left"}});
        controller->handleRotationDirection(request);

        fakeit::Verify(Method(processingThreadMock, setDirection)).Once();
        fakeit::Verify(Method(processingThreadMock, setDirection).Using(RotationDirection::left));
    }
    
    SECTION("handleReleaseStatus() => direction field is missing")
    {
        auto request = new Request("setReleaseStatus", {{}});

        try {
            controller->handleReleaseStatus(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field released");
        }
    }

    SECTION("handleReleaseStatus() => speed field is not boolean")
    {
        auto request = new Request("setReleaseStatus", {{"released", 12893}});

        try {
            controller->handleReleaseStatus(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field released is not boolean");
        }
    }

    SECTION("handleReleaseStatus() => processing thread called => right")
    {
        auto request = new Request("setReleaseStatus", {{"released", true}});
        controller->handleReleaseStatus(request);

        fakeit::Verify(Method(processingThreadMock, setReleased)).Once();
        fakeit::Verify(Method(processingThreadMock, setReleased).Using(true));
    }

    SECTION("handleReleaseStatus() => processing thread called => left")
    {
        auto request = new Request("setReleaseStatus", {{"released", false}});
        controller->handleReleaseStatus(request);

        fakeit::Verify(Method(processingThreadMock, setReleased)).Once();
        fakeit::Verify(Method(processingThreadMock, setReleased).Using(false));
    }

    SECTION("setOperationMode() => mode field is missing")
    {
        auto request = new Request("setOperationMode", {{}});

        try {
            controller->handleOperationMode(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field mode");
        }
    }

    SECTION("setOperationMode() => speed field is not string")
    {
        auto request = new Request("setOperationMode", {{"mode", 12893}});

        try {
            controller->handleOperationMode(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field mode is not a string");
        }
    }

    SECTION("setOperationMode() => processing thread called => static max speed")
    {
        fakeit::When(Method(processingThreadMock, setOperationMode)).AlwaysDo([] (OperationMode* mode) {
            REQUIRE(mode != nullptr);
            CHECK(mode->getIdentifier() == "staticMaxSpeed");
        });

        auto request = new Request("setOperationMode", {{"mode", "staticMaxSpeed"}});
        controller->handleOperationMode(request);

        fakeit::Verify(Method(processingThreadMock, setOperationMode)).Once();
    }

    SECTION("setOperationMode() => processing thread called => regular spiral")
    {
        fakeit::When(Method(processingThreadMock, setOperationMode)).AlwaysDo([] (OperationMode* mode) {
            REQUIRE(mode != nullptr);
            CHECK(mode->getIdentifier() == "regularSpiral");
        });

        auto request = new Request("setOperationMode", {{"mode", "regularSpiral"}});
        controller->handleOperationMode(request);

        fakeit::Verify(Method(processingThreadMock, setOperationMode)).Once();
    }


    SECTION("setOperationMode() => speed field is not string")
    {
        auto request = new Request("setOperationMode", {{"mode", "crazyMode"}});

        try {
            controller->handleOperationMode(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Invalid mode identifier given");
        }
    }

    SECTION("Correct heartbeat handling")
    {
        auto request = new Request("heartbeat", {});
        HeartbeatResponse* response = controller->handleHeartbeat();

        REQUIRE(response != nullptr);
    }
}