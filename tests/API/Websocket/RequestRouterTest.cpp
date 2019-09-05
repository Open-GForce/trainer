#include "../../../../catch2/catch.hpp"
#include "../../../../fake_it/single_header/catch/fakeit.hpp"
#include "../../../src/API/Websocket/RequestRouter.hpp"
#include "../../../src/Utils/Assertions/AssertionFailedException.hpp"
#include "../../../src/Processing/ProcessingStatus.hpp"

using namespace GForce::API::Websocket;
using namespace GForce::Utils::Assertions;

TEST_CASE( "Request router tests", "[Websocket]" )
{
    fakeit::Mock<OperationsController> operationsControllerMock;
    fakeit::Fake(Method(operationsControllerMock, handleSpeedLimit));
    fakeit::Fake(Method(operationsControllerMock, handleRotationDirection));
    fakeit::Fake(Method(operationsControllerMock, handleReleaseStatus));
    OperationsController* operationsController = &operationsControllerMock.get();

    auto router = new RequestRouter(operationsController);

    nlohmann::json correctMessage = {
            {"type", "replace_me"},
            {"data", {
                 {"test", 123}
            }}
    };

    SECTION("Missing type")
    {
        nlohmann::json data = correctMessage;
        data.erase("type");

        try {
            router->handle(data.dump());
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field type");
        }
    }

    SECTION("Missing data")
    {
        nlohmann::json data = correctMessage;
        data.erase("data");

        try {
            router->handle(data.dump());
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field data");
        }
    }

    SECTION("Type not string")
    {
        nlohmann::json data = correctMessage;
        data["type"] = 18.182;

        try {
            router->handle(data.dump());
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field type is not a string");
        }
    }

    SECTION("Data not object")
    {
        nlohmann::json data = correctMessage;
        data["data"] = "abc";

        try {
            router->handle(data.dump());
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field data is not an object");
        }
    }

    SECTION("OperationsController->handleSpeedLimit() called")
    {
        nlohmann::json data = correctMessage;
        data["type"] = "setMaxSpeed";

        fakeit::When(Method(operationsControllerMock, handleSpeedLimit)).AlwaysDo([] (Request* request) {
            CHECK(request->getType() == "setMaxSpeed");
        });

        router->handle(data.dump());
        fakeit::Verify(Method(operationsControllerMock, handleSpeedLimit)).Once();
    }

    SECTION("OperationsController->handleRotationDirection() called")
    {
        nlohmann::json data = correctMessage;
        data["type"] = "setRotationDirection";

        fakeit::When(Method(operationsControllerMock, handleSpeedLimit)).AlwaysDo([] (Request* request) {
            CHECK(request->getType() == "setRotationDirection");
        });

        router->handle(data.dump());
        fakeit::Verify(Method(operationsControllerMock, handleRotationDirection)).Once();
    }

    SECTION("OperationsController->handleReleaseStatus() called")
    {
        nlohmann::json data = correctMessage;
        data["type"] = "setReleaseStatus";

        fakeit::When(Method(operationsControllerMock, handleSpeedLimit)).AlwaysDo([] (Request* request) {
            CHECK(request->getType() == "setReleaseStatus");
        });

        router->handle(data.dump());
        fakeit::Verify(Method(operationsControllerMock, handleReleaseStatus)).Once();
    }
}