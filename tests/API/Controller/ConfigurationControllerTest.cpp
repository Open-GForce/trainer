#include "../../../includes/catch2/catch.hpp"
#include "../../../includes/fake_it/single_header/catch/fakeit.hpp"
#include "../../../src/API/Controller/ConfigurationController.hpp"
#include "../../../src/Configuration/ConfigRepository.hpp"
#include "../../../src/Utils/Assertions/AssertionFailedException.hpp"

using namespace GForce::API::Controller;
using namespace GForce::Configuration;
using namespace GForce::Utils::Assertions;

TEST_CASE( "ConfigurationController tests", "[Controller]" )
{
    fakeit::Mock<ProcessingThread> processingThreadMock;
    fakeit::Fake(Method(processingThreadMock, reloadUserConfig));
    ProcessingThread* processingThread = &processingThreadMock.get();

    fakeit::Mock<ConfigRepository> configRepositoryMock;
    fakeit::Fake(Method(configRepositoryMock, loadUserSettings));
    fakeit::Fake(Method(configRepositoryMock, saveUserSettings));
    ConfigRepository* configRepository = &configRepositoryMock.get();

    auto controller = new ConfigurationController(processingThread, configRepository);

    nlohmann::json correctData = {
            {"min", 1673},
            {"max", 4434}
    };

    SECTION("setInnerBrakeRange() => min field is missing")
    {
        auto data = correctData;
        data.erase("min");

        auto request = new Request("test", data);

        try {
            controller->setInnerBrakeRange(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field min");
        }
    }

    SECTION("setInnerBrakeRange() => max field is missing")
    {
        auto data = correctData;
        data.erase("max");

        auto request = new Request("test", data);

        try {
            controller->setInnerBrakeRange(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field max");
        }
    }

    SECTION("setInnerBrakeRange() => min field not a number")
    {
        auto data = correctData;
        data["min"] = "abc";

        auto request = new Request("test", data);

        try {
            controller->setInnerBrakeRange(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field min is not a number");
        }
    }

    SECTION("setInnerBrakeRange() => max field not a number")
    {
        auto data = correctData;
        data["max"] = "abc";

        auto request = new Request("test", data);

        try {
            controller->setInnerBrakeRange(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field max is not a number");
        }
    }
    
    SECTION("setInnerBrakeRange() => config merged and reloaded")
    {
        fakeit::When(Method(configRepositoryMock, loadUserSettings)).AlwaysReturn(new UserSettings(new Range(1000, 2000), new Range(3000, 4000)));

        fakeit::When(Method(configRepositoryMock, saveUserSettings)).AlwaysDo([] (UserSettings* settings) {
            CHECK(settings->getInnerBrakeRange()->getMin() == 1673);
            CHECK(settings->getInnerBrakeRange()->getMax() == 4434);
            CHECK(settings->getOuterBrakeRange()->getMin() == 3000);
            CHECK(settings->getOuterBrakeRange()->getMax() == 4000);
        });

        fakeit::When(Method(processingThreadMock, reloadUserConfig)).AlwaysDo([] (UserSettings* settings) {
            CHECK(settings->getInnerBrakeRange()->getMin() == 1673);
            CHECK(settings->getInnerBrakeRange()->getMax() == 4434);
            CHECK(settings->getOuterBrakeRange()->getMin() == 3000);
            CHECK(settings->getOuterBrakeRange()->getMax() == 4000);
        });

        auto request = new Request("test", correctData);
        controller->setInnerBrakeRange(request);
        
        fakeit::Verify(Method(configRepositoryMock, saveUserSettings)).Once();
        fakeit::Verify(Method(processingThreadMock, reloadUserConfig)).Once();
    }

    SECTION("setOuterBrakeRange() => min field is missing")
    {
        auto data = correctData;
        data.erase("min");

        auto request = new Request("test", data);

        try {
            controller->setOuterBrakeRange(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field min");
        }
    }

    SECTION("setOuterBrakeRange() => max field is missing")
    {
        auto data = correctData;
        data.erase("max");

        auto request = new Request("test", data);

        try {
            controller->setOuterBrakeRange(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field max");
        }
    }

    SECTION("setOuterBrakeRange() => min field not a number")
    {
        auto data = correctData;
        data["min"] = "abc";

        auto request = new Request("test", data);

        try {
            controller->setOuterBrakeRange(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field min is not a number");
        }
    }

    SECTION("setOuterBrakeRange() => max field not a number")
    {
        auto data = correctData;
        data["max"] = "abc";

        auto request = new Request("test", data);

        try {
            controller->setOuterBrakeRange(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field max is not a number");
        }
    }

    SECTION("setOuterBrakeRange() => config merged and reloaded")
    {
        fakeit::When(Method(configRepositoryMock, loadUserSettings)).AlwaysReturn(new UserSettings(new Range(1000, 2000), new Range(3000, 4000)));

        fakeit::When(Method(configRepositoryMock, saveUserSettings)).AlwaysDo([] (UserSettings* settings) {
            CHECK(settings->getInnerBrakeRange()->getMin() == 1000);
            CHECK(settings->getInnerBrakeRange()->getMax() == 2000);
            CHECK(settings->getOuterBrakeRange()->getMin() == 1673);
            CHECK(settings->getOuterBrakeRange()->getMax() == 4434);
        });

        fakeit::When(Method(processingThreadMock, reloadUserConfig)).AlwaysDo([] (UserSettings* settings) {
            CHECK(settings->getInnerBrakeRange()->getMin() == 1000);
            CHECK(settings->getInnerBrakeRange()->getMax() == 2000);
            CHECK(settings->getOuterBrakeRange()->getMin() == 1673);
            CHECK(settings->getOuterBrakeRange()->getMax() == 4434);
        });

        auto request = new Request("test", correctData);
        controller->setOuterBrakeRange(request);

        fakeit::Verify(Method(configRepositoryMock, saveUserSettings)).Once();
        fakeit::Verify(Method(processingThreadMock, reloadUserConfig)).Once();
    }

}