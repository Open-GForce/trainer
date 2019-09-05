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
    ProcessingThread* processingThread = &processingThreadMock.get();

    auto controller = new OperationsController(processingThread);

    SECTION("Speed field is missing")
    {
        auto request = new Request("setMaxSpeed", {{}});

        try {
            controller->handleSpeedLimit(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field speed");
        }
    }

    SECTION("Speed field is not number")
    {
        auto request = new Request("setMaxSpeed", {{"speed", "abc"}});

        try {
            controller->handleSpeedLimit(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field speed is not a number");
        }
    }

    SECTION("Processing thread called")
    {
        auto request = new Request("setMaxSpeed", {{"speed", 348.31}});
        controller->handleSpeedLimit(request);

        fakeit::Verify(Method(processingThreadMock, setMaxSpeed)).Once();
        fakeit::Verify(Method(processingThreadMock, setMaxSpeed).Using(348.31));
    }
}