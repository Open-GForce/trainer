#include <catch2/catch.hpp>
#include <fakeit.hpp>
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

    nlohmann::json correctRangeData = {
            {"min", 1673},
            {"max", 4434}
    };

    nlohmann::json correctRadiusData = {
            {"rotationRadius", 2.53}
    };

    nlohmann::json correctSoftStartData = {
            {"speed", 750.5},
            {"acceleration", 1555}
    };

    nlohmann::json correctAccelerationStagesData = {
            {"stages", {}},
            {"mode", "differential"}
    };
    correctAccelerationStagesData["stages"].push_back({{"speed", 100}, {"acceleration", 2500}});
    correctAccelerationStagesData["stages"].push_back({{"speed", 200}, {"acceleration", 3400}});

    SECTION("setInnerBrakeRange() => min field is missing")
    {
        auto data = correctRangeData;
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
        auto data = correctRangeData;
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
        auto data = correctRangeData;
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
        auto data = correctRangeData;
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
        fakeit::When(Method(configRepositoryMock, loadUserSettings)).AlwaysReturn(new UserSettings(
                new Range(1000, 2000),
                new Range(3000, 4000),
                5.0,
                100,
                1000,
                {AccelerationStage(500, 1250)}, AccelerationMode::targetSpeed, false));

        fakeit::When(Method(configRepositoryMock, saveUserSettings)).AlwaysDo([] (UserSettings* settings) {
            CHECK(settings->getInnerBrakeRange()->getMin() == 1673);
            CHECK(settings->getInnerBrakeRange()->getMax() == 4434);
            CHECK(settings->getOuterBrakeRange()->getMin() == 3000);
            CHECK(settings->getOuterBrakeRange()->getMax() == 4000);
            CHECK(settings->getSoftStartSpeed() == 100);
            CHECK(settings->getSoftStartAcceleration() == 1000);
            CHECK(settings->getAccelerationStages().size() == 1);
            CHECK(settings->getAccelerationStages().front().getSpeed() == 500);
            CHECK(settings->getAccelerationStages().front().getAcceleration() == 1250);
        });

        fakeit::When(Method(processingThreadMock, reloadUserConfig)).AlwaysDo([] (UserSettings* settings) {
            CHECK(settings->getInnerBrakeRange()->getMin() == 1673);
            CHECK(settings->getInnerBrakeRange()->getMax() == 4434);
            CHECK(settings->getOuterBrakeRange()->getMin() == 3000);
            CHECK(settings->getOuterBrakeRange()->getMax() == 4000);
            CHECK(settings->getSoftStartSpeed() == 100);
            CHECK(settings->getSoftStartAcceleration() == 1000);
            CHECK(settings->getAccelerationStages().size() == 1);
            CHECK(settings->getAccelerationStages().front().getSpeed() == 500);
            CHECK(settings->getAccelerationStages().front().getAcceleration() == 1250);
        });

        auto request = new Request("test", correctRangeData);
        controller->setInnerBrakeRange(request);
        
        fakeit::Verify(Method(configRepositoryMock, saveUserSettings)).Once();
        fakeit::Verify(Method(processingThreadMock, reloadUserConfig)).Once();
    }

    SECTION("setOuterBrakeRange() => min field is missing")
    {
        auto data = correctRangeData;
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
        auto data = correctRangeData;
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
        auto data = correctRangeData;
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
        auto data = correctRangeData;
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
        fakeit::When(Method(configRepositoryMock, loadUserSettings)).AlwaysReturn(new UserSettings(
                new Range(1000, 2000),
                new Range(3000, 4000),
                5.0,
                100,
                1000,
                {AccelerationStage(500, 1250)}, AccelerationMode::targetSpeed, false));

        fakeit::When(Method(configRepositoryMock, saveUserSettings)).AlwaysDo([] (UserSettings* settings) {
            CHECK(settings->getInnerBrakeRange()->getMin() == 1000);
            CHECK(settings->getInnerBrakeRange()->getMax() == 2000);
            CHECK(settings->getOuterBrakeRange()->getMin() == 1673);
            CHECK(settings->getOuterBrakeRange()->getMax() == 4434);
            CHECK(settings->getSoftStartSpeed() == 100);
            CHECK(settings->getSoftStartAcceleration() == 1000);
            CHECK(settings->getAccelerationStages().size() == 1);
            CHECK(settings->getAccelerationStages().front().getSpeed() == 500);
            CHECK(settings->getAccelerationStages().front().getAcceleration() == 1250);

        });

        fakeit::When(Method(processingThreadMock, reloadUserConfig)).AlwaysDo([] (UserSettings* settings) {
            CHECK(settings->getInnerBrakeRange()->getMin() == 1000);
            CHECK(settings->getInnerBrakeRange()->getMax() == 2000);
            CHECK(settings->getOuterBrakeRange()->getMin() == 1673);
            CHECK(settings->getOuterBrakeRange()->getMax() == 4434);
            CHECK(settings->getSoftStartSpeed() == 100);
            CHECK(settings->getSoftStartAcceleration() == 1000);
            CHECK(settings->getAccelerationStages().size() == 1);
            CHECK(settings->getAccelerationStages().front().getSpeed() == 500);
            CHECK(settings->getAccelerationStages().front().getAcceleration() == 1250);
        });

        auto request = new Request("test", correctRangeData);
        controller->setOuterBrakeRange(request);

        fakeit::Verify(Method(configRepositoryMock, saveUserSettings)).Once();
        fakeit::Verify(Method(processingThreadMock, reloadUserConfig)).Once();
    }

    SECTION("setRotationRadius() => rotationRadius field is missing")
    {
        auto data = correctRadiusData;
        data.erase("rotationRadius");

        auto request = new Request("test", data);

        try {
            controller->setRotationRadius(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field rotationRadius");
        }
    }

    SECTION("setRotationRadius() => rotationRadius field not a number")
    {
        auto data = correctRadiusData;
        data["rotationRadius"] = "abc";

        auto request = new Request("test", data);

        try {
            controller->setRotationRadius(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field rotationRadius is not a number");
        }
    }

    SECTION("setRotationRadius() => config merged and reloaded")
    {
        fakeit::When(Method(configRepositoryMock, loadUserSettings)).AlwaysReturn(new UserSettings(
                new Range(1000, 2000),
                new Range(3000, 4000),
                5.0,
                100,
                1000,
                {AccelerationStage(500, 1250)}, AccelerationMode::targetSpeed, false));

        fakeit::When(Method(configRepositoryMock, saveUserSettings)).AlwaysDo([] (UserSettings* settings) {
            CHECK(settings->getInnerBrakeRange()->getMin() == 1000);
            CHECK(settings->getInnerBrakeRange()->getMax() == 2000);
            CHECK(settings->getOuterBrakeRange()->getMin() == 3000);
            CHECK(settings->getOuterBrakeRange()->getMax() == 4000);
            CHECK(settings->getRotationRadius() == 2.53);
            CHECK(settings->getSoftStartSpeed() == 100);
            CHECK(settings->getSoftStartAcceleration() == 1000);
            CHECK(settings->getAccelerationStages().size() == 1);
            CHECK(settings->getAccelerationStages().front().getSpeed() == 500);
            CHECK(settings->getAccelerationStages().front().getAcceleration() == 1250);
        });

        fakeit::When(Method(processingThreadMock, reloadUserConfig)).AlwaysDo([] (UserSettings* settings) {
            CHECK(settings->getInnerBrakeRange()->getMin() == 1000);
            CHECK(settings->getInnerBrakeRange()->getMax() == 2000);
            CHECK(settings->getOuterBrakeRange()->getMin() == 3000);
            CHECK(settings->getOuterBrakeRange()->getMax() == 4000);
            CHECK(settings->getRotationRadius() == 2.53);
            CHECK(settings->getSoftStartSpeed() == 100);
            CHECK(settings->getSoftStartAcceleration() == 1000);
            CHECK(settings->getAccelerationStages().size() == 1);
            CHECK(settings->getAccelerationStages().front().getSpeed() == 500);
            CHECK(settings->getAccelerationStages().front().getAcceleration() == 1250);
        });

        auto request = new Request("test", correctRadiusData);
        controller->setRotationRadius(request);

        fakeit::Verify(Method(configRepositoryMock, saveUserSettings)).Once();
        fakeit::Verify(Method(processingThreadMock, reloadUserConfig)).Once();
    }

    SECTION("setSoftStart() => speed field is missing")
    {
        auto data = correctSoftStartData;
        data.erase("speed");

        auto request = new Request("test", data);

        try {
            controller->setSoftStart(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field speed");
        }
    }

    SECTION("setRotationRadius() => rotationRadius field not a number")
    {
        auto data = correctSoftStartData;
        data["speed"] = "abc";

        auto request = new Request("test", data);

        try {
            controller->setSoftStart(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field speed is not a number");
        }
    }

    SECTION("setSoftStart() => acceleration field is missing")
    {
        auto data = correctSoftStartData;
        data.erase("acceleration");

        auto request = new Request("test", data);

        try {
            controller->setSoftStart(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field acceleration");
        }
    }

    SECTION("setRotationRadius() => acceleration field not a number")
    {
        auto data = correctSoftStartData;
        data["acceleration"] = "abc";

        auto request = new Request("test", data);

        try {
            controller->setSoftStart(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field acceleration is not a number");
        }
    }

    SECTION("setAccelerationStages() => acceleration field is missing")
    {
        auto data = correctAccelerationStagesData;
        data.erase("stages");

        auto request = new Request("test", data);

        try {
            controller->setAccelerationStages(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field stages");
        }
    }

    SECTION("setAccelerationStages() => mode field is missing")
    {
        auto data = correctAccelerationStagesData;
        data.erase("mode");

        auto request = new Request("test", data);

        try {
            controller->setAccelerationStages(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field mode");
        }
    }

    SECTION("setAccelerationStages() => mode field contains invalid value => default used")
    {
        auto data = correctAccelerationStagesData;
        data["mode"] = "invalidValue";

        fakeit::When(Method(configRepositoryMock, loadUserSettings)).AlwaysReturn(new UserSettings(
                new Range(1000, 2000),
                new Range(3000, 4000),
                5.0,
                100,
                1000,
                {AccelerationStage(500, 1250)}, AccelerationMode::targetSpeed, false));

        fakeit::When(Method(configRepositoryMock, saveUserSettings)).AlwaysDo([] (UserSettings* settings) {
            CHECK(settings->getAccelerationMode() == AccelerationMode::targetSpeed);
        });

        auto request = new Request("test", data);
        controller->setAccelerationStages(request);

        fakeit::Verify(Method(configRepositoryMock, saveUserSettings)).Once();
    }

    SECTION("setAdaptiveAccelerationUIToggle() => toggle not a boolean")
    {
        auto data = correctSoftStartData;
        data["activateAdaptiveAcceleration"] = "abc";

        auto request = new Request("test", data);

        try {
            controller->setUserInterfaceSettings(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field activateAdaptiveAcceleration is not boolean");
        }
    }

    SECTION("setAdaptiveAccelerationUIToggle() => toggle not a boolean")
    {
        auto data = correctSoftStartData;
        data.erase("activateAdaptiveAcceleration");

        auto request = new Request("test", data);

        try {
            controller->setUserInterfaceSettings(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field activateAdaptiveAcceleration");
        }
    }

    SECTION("setAccelerationStages() => array field not an array")
    {
        auto data = correctAccelerationStagesData;
        data.erase("stages");
        data["stages"] = "abc";

        auto request = new Request("test", data);

        try {
            controller->setAccelerationStages(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field stages is not an array");
        }
    }

    SECTION("setAccelerationStages() => mode field not a string")
    {
        auto data = correctAccelerationStagesData;
        data.erase("mode");
        data["mode"] = {1, 2, 3};

        auto request = new Request("test", data);

        try {
            controller->setAccelerationStages(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field mode is not a string");
        }
    }

    SECTION("setAccelerationStages() => field speed missing of item")
    {
        nlohmann::json data = correctAccelerationStagesData;
        data.erase("stages");
        data["stages"] = {};

        data["stages"].push_back({{"acceleration", 2500}});
        data["stages"].push_back({{"speed", 200}, {"acceleration", 3400}});

        auto request = new Request("test", data);

        try {
            controller->setAccelerationStages(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field speed");
        }
    }

    SECTION("setAccelerationStages() => field acceleration missing of item")
    {
        nlohmann::json data = correctAccelerationStagesData;
        data.erase("stages");
        data["stages"] = {};

        data["stages"].push_back({{"speed", 100}});
        data["stages"].push_back({{"speed", 200}, {"acceleration", 3400}});

        auto request = new Request("test", data);

        try {
            controller->setAccelerationStages(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field acceleration");
        }
    }

    SECTION("setAccelerationStages() => speed field not a number")
    {
        nlohmann::json data = correctAccelerationStagesData;
        data.erase("stages");
        data["stages"] = {};

        data["stages"].push_back({{"speed", 100}, {"acceleration", 2500}});
        data["stages"].push_back({{"speed", "abc"}, {"acceleration", 3400}});

        auto request = new Request("test", data);

        try {
            controller->setAccelerationStages(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field speed is not a number");
        }
    }

    SECTION("setAccelerationStages() => acceleration field not a number")
    {
        nlohmann::json data = correctAccelerationStagesData;
        data.erase("stages");
        data["stages"] = {};

        data["stages"].push_back({{"speed", 100}, {"acceleration", 2500}});
        data["stages"].push_back({{"speed", 200}, {"acceleration", "abc"}});

        auto request = new Request("test", data);

        try {
            controller->setAccelerationStages(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field acceleration is not a number");
        }
    }

    SECTION("setSoftStart() => config merged and reloaded")
    {
        fakeit::When(Method(configRepositoryMock, loadUserSettings)).AlwaysReturn(new UserSettings(
                new Range(1000, 2000),
                new Range(3000, 4000),
                5.0,
                100,
                1000,
                {AccelerationStage(500, 1250)}, AccelerationMode::targetSpeed, false));

        fakeit::When(Method(configRepositoryMock, saveUserSettings)).AlwaysDo([] (UserSettings* settings) {
            CHECK(settings->getInnerBrakeRange()->getMin() == 1000);
            CHECK(settings->getInnerBrakeRange()->getMax() == 2000);
            CHECK(settings->getOuterBrakeRange()->getMin() == 3000);
            CHECK(settings->getOuterBrakeRange()->getMax() == 4000);
            CHECK(settings->getRotationRadius() == 5.0);
            CHECK(settings->getSoftStartSpeed() == 750.5);
            CHECK(settings->getSoftStartAcceleration() == 1555);
            CHECK(settings->getAccelerationStages().size() == 1);
            CHECK(settings->getAccelerationStages().front().getSpeed() == 500);
            CHECK(settings->getAccelerationStages().front().getAcceleration() == 1250);
            CHECK(!settings->isAdaptiveAccelerationUIActivated());
            CHECK(settings->getAccelerationMode() == AccelerationMode::targetSpeed);
        });

        fakeit::When(Method(processingThreadMock, reloadUserConfig)).AlwaysDo([] (UserSettings* settings) {
            CHECK(settings->getInnerBrakeRange()->getMin() == 1000);
            CHECK(settings->getInnerBrakeRange()->getMax() == 2000);
            CHECK(settings->getOuterBrakeRange()->getMin() == 3000);
            CHECK(settings->getOuterBrakeRange()->getMax() == 4000);
            CHECK(settings->getRotationRadius() == 5.0);
            CHECK(settings->getSoftStartSpeed() == 750.5);
            CHECK(settings->getSoftStartAcceleration() == 1555);
            CHECK(settings->getAccelerationStages().size() == 1);
            CHECK(settings->getAccelerationStages().front().getSpeed() == 500);
            CHECK(settings->getAccelerationStages().front().getAcceleration() == 1250);
            CHECK(!settings->isAdaptiveAccelerationUIActivated());
            CHECK(settings->getAccelerationMode() == AccelerationMode::targetSpeed);
        });

        auto request = new Request("test", correctSoftStartData);
        controller->setSoftStart(request);

        fakeit::Verify(Method(configRepositoryMock, saveUserSettings)).Once();
        fakeit::Verify(Method(processingThreadMock, reloadUserConfig)).Once();
    }

    SECTION("setAccelerationStages() => config merged and reloaded")
    {
        fakeit::When(Method(configRepositoryMock, loadUserSettings)).AlwaysReturn(new UserSettings(
                new Range(1000, 2000),
                new Range(3000, 4000),
                5.0,
                100,
                1000,
                {AccelerationStage(500, 1250)}, AccelerationMode::targetSpeed, false));

        fakeit::When(Method(configRepositoryMock, saveUserSettings)).AlwaysDo([] (UserSettings* settings) {
            CHECK(settings->getInnerBrakeRange()->getMin() == 1000);
            CHECK(settings->getInnerBrakeRange()->getMax() == 2000);
            CHECK(settings->getOuterBrakeRange()->getMin() == 3000);
            CHECK(settings->getOuterBrakeRange()->getMax() == 4000);
            CHECK(settings->getRotationRadius() == 5.0);
            CHECK(settings->getSoftStartSpeed() == 100);
            CHECK(settings->getSoftStartAcceleration() == 1000);
            CHECK(settings->getAccelerationStages().size() == 2);
            CHECK(settings->getAccelerationStages().front().getSpeed() == 100);
            CHECK(settings->getAccelerationStages().front().getAcceleration() == 2500);
            CHECK(settings->getAccelerationStages().back().getSpeed() == 200);
            CHECK(settings->getAccelerationStages().back().getAcceleration() == 3400);
            CHECK(settings->getAccelerationMode() == AccelerationMode::differential);
        });

        fakeit::When(Method(processingThreadMock, reloadUserConfig)).AlwaysDo([] (UserSettings* settings) {
            CHECK(settings->getInnerBrakeRange()->getMin() == 1000);
            CHECK(settings->getInnerBrakeRange()->getMax() == 2000);
            CHECK(settings->getOuterBrakeRange()->getMin() == 3000);
            CHECK(settings->getOuterBrakeRange()->getMax() == 4000);
            CHECK(settings->getRotationRadius() == 5.0);
            CHECK(settings->getSoftStartSpeed() == 100);
            CHECK(settings->getSoftStartAcceleration() == 1000);
            CHECK(settings->getAccelerationStages().size() == 2);
            CHECK(settings->getAccelerationStages().front().getSpeed() == 100);
            CHECK(settings->getAccelerationStages().front().getAcceleration() == 2500);
            CHECK(settings->getAccelerationStages().back().getSpeed() == 200);
            CHECK(settings->getAccelerationStages().back().getAcceleration() == 3400);
            CHECK(settings->getAccelerationMode() == AccelerationMode::differential);
        });

        auto request = new Request("test", correctAccelerationStagesData);
        controller->setAccelerationStages(request);

        fakeit::Verify(Method(configRepositoryMock, saveUserSettings)).Once();
        fakeit::Verify(Method(processingThreadMock, reloadUserConfig)).Once();
    }
}