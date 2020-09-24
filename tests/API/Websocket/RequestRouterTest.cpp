#include "../../../includes/catch2/catch.hpp"
#include "../../../includes/fake_it/single_header/catch/fakeit.hpp"
#include "../../../src/API/Websocket/RequestRouter.hpp"
#include "../../../src/API/Controller/HeartbeatResponse.hpp"
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
    fakeit::Fake(Method(operationsControllerMock, handleOperationMode));
    OperationsController* operationsController = &operationsControllerMock.get();

    fakeit::Mock<ConfigurationController> configControllerMock;
    fakeit::Fake(Method(configControllerMock, setInnerBrakeRange));
    fakeit::Fake(Method(configControllerMock, setOuterBrakeRange));
    fakeit::Fake(Method(configControllerMock, setSoftStart));
    fakeit::Fake(Method(configControllerMock, setAccelerationStages));
    ConfigurationController* configController = &configControllerMock.get();

    auto router = new RequestRouter(operationsController, configController);

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


    SECTION("OperationsController->handleOperationMode() called")
    {
        nlohmann::json data = correctMessage;
        data["type"] = "setOperationMode";

        fakeit::When(Method(operationsControllerMock, handleOperationMode)).AlwaysDo([] (Request* request) {
            CHECK(request->getType() == "setOperationMode");
        });

        router->handle(data.dump());
        fakeit::Verify(Method(operationsControllerMock, handleOperationMode)).Once();
    }

    SECTION("ConfigurationController->setInnerBrakeRange() called")
    {
        nlohmann::json data = correctMessage;
        data["type"] = "setInnerBrakeRange";

        fakeit::When(Method(configControllerMock, setInnerBrakeRange)).AlwaysDo([] (Request* request) {
            CHECK(request->getType() == "setInnerBrakeRange");
        });

        router->handle(data.dump());
        fakeit::Verify(Method(configControllerMock, setInnerBrakeRange)).Once();
    }

    SECTION("ConfigurationController->setOuterBrakeRange() called")
    {
        nlohmann::json data = correctMessage;
        data["type"] = "setOuterBrakeRange";

        fakeit::When(Method(configControllerMock, setOuterBrakeRange)).AlwaysDo([] (Request* request) {
            CHECK(request->getType() == "setOuterBrakeRange");
        });

        router->handle(data.dump());
        fakeit::Verify(Method(configControllerMock, setOuterBrakeRange)).Once();
    }

    SECTION("ConfigurationController->setRotationRadius() called")
    {
        nlohmann::json data = correctMessage;
        data["type"] = "setRotationRadius";

        fakeit::When(Method(configControllerMock, setRotationRadius)).AlwaysDo([] (Request* request) {
            CHECK(request->getType() == "setRotationRadius");
        });

        router->handle(data.dump());
        fakeit::Verify(Method(configControllerMock, setRotationRadius)).Once();
    }

    SECTION("ConfigurationController->setSoftStart() called")
    {
        nlohmann::json data = correctMessage;
        data["type"] = "setSoftStart";

        fakeit::When(Method(configControllerMock, setSoftStart)).AlwaysDo([] (Request* request) {
            CHECK(request->getType() == "setSoftStart");
        });

        router->handle(data.dump());
        fakeit::Verify(Method(configControllerMock, setSoftStart)).Once();
    }

    SECTION("ConfigurationController->setAccelerationStages() called")
    {
        nlohmann::json data = correctMessage;
        data["type"] = "setAccelerationStages";

        fakeit::When(Method(configControllerMock, setAccelerationStages)).AlwaysDo([] (Request* request) {
            CHECK(request->getType() == "setAccelerationStages");
        });

        router->handle(data.dump());
        fakeit::Verify(Method(configControllerMock, setAccelerationStages)).Once();
    }

    SECTION("ConfigurationController->setAccelerationStages() called")
    {
        nlohmann::json data = correctMessage;
        data["type"] = "setUserInterfaceSettings";

        fakeit::When(Method(configControllerMock, setUserInterfaceSettings)).AlwaysDo([] (Request* request) {
            CHECK(request->getType() == "setUserInterfaceSettings");
        });

        router->handle(data.dump());
        fakeit::Verify(Method(configControllerMock, setUserInterfaceSettings)).Once();
    }

    SECTION("ConfigurationController->loadUserSettings() called")
    {
        nlohmann::json data = correctMessage;
        data["type"] = "getUserSettings";

        fakeit::When(Method(configControllerMock, getUserSettings)).AlwaysDo([] () {
            return new UserSettings(new Range(1000, 2000), new Range(3000, 4000), 5.0, 0, 0,
                                    std::list<AccelerationStage>(), AccelerationMode::targetSpeed, false);
        });

        auto response = router->handle(data.dump());

        REQUIRE(response != nullptr);
        CHECK(response->toResponse()->getType() == "userSettings");
    }

    SECTION("ConfigurationController->handleHeartbeat() called")
    {
        nlohmann::json data = correctMessage;
        data["type"] = "heartbeat";

        fakeit::When(Method(operationsControllerMock, handleHeartbeat)).AlwaysDo([] () {
            return new HeartbeatResponse();
        });

        auto response = router->handle(data.dump());

        REQUIRE(response != nullptr);
        CHECK(response->toResponse()->getType() == "heartbeat");
    }
}