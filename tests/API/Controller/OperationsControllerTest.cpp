#include "../../../../catch2/catch.hpp"
#include "../../../../fake_it/single_header/catch/fakeit.hpp"
#include "../../../src/API/Controller/OperationsController.hpp"
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
}