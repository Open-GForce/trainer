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

    nlohmann::json correctInnerBrakeConfiguration = {
            {"min", 1673},
            {"max", 4434}
    };

    nlohmann::json correctOuterBrakeConfiguration = {
            {"min", 1673},
            {"max", 4434},
            {"deactivated", true},
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

    SECTION("getUserSettings() => name field is missing")
    {
        auto request = new Request("test", {});

        try {
            controller->getUserSettings(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field name");
        }
    }

    SECTION("getUserSettings() => name field not a string")
    {
        auto request = new Request("test", {
            {"name", 5.0}
        });

        try {
            controller->getUserSettings(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field name is not a string");
        }
    }

    SECTION("getUserSettings() => repository called")
    {
        fakeit::When(Method(configRepositoryMock, loadUserSettings)).AlwaysReturn(new UserSettings(
                new Range(1000, 2000),
                new Range(3000, 4000),
                5.0,
                100,
                1000,
                {AccelerationStage(500, 1250)}, AccelerationMode::targetSpeed, false, true));

        auto request = new Request("test", {
                {"name", "example"}
        });

        auto result = controller->getUserSettings(request);

        REQUIRE(result != nullptr);
        CHECK(result->getSoftStartSpeed() == 100);

        fakeit::Verify(Method(configRepositoryMock, loadUserSettings)).Once();
        fakeit::Verify(Method(configRepositoryMock, loadUserSettings).Using("example"));
    }

    SECTION("setInnerBrakeRange() => min field is missing")
    {
        auto data = correctInnerBrakeConfiguration;
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
        auto data = correctInnerBrakeConfiguration;
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
        auto data = correctInnerBrakeConfiguration;
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
        auto data = correctInnerBrakeConfiguration;
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
                {AccelerationStage(500, 1250)}, AccelerationMode::targetSpeed, false, true));

        fakeit::When(Method(configRepositoryMock, saveUserSettings)).AlwaysDo([] (std::string name, UserSettings* settings) {
            CHECK(name == "default");
            CHECK(settings->getInnerBrakeRange()->getMin() == 1673);
            CHECK(settings->getInnerBrakeRange()->getMax() == 4434);
            CHECK(settings->getOuterBrakeRange()->getMin() == 3000);
            CHECK(settings->getOuterBrakeRange()->getMax() == 4000);
            CHECK(settings->isOuterBrakeDeactivated());
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
            CHECK(settings->isOuterBrakeDeactivated());
            CHECK(settings->getSoftStartSpeed() == 100);
            CHECK(settings->getSoftStartAcceleration() == 1000);
            CHECK(settings->getAccelerationStages().size() == 1);
            CHECK(settings->getAccelerationStages().front().getSpeed() == 500);
            CHECK(settings->getAccelerationStages().front().getAcceleration() == 1250);
        });

        auto request = new Request("test", correctInnerBrakeConfiguration);
        controller->setInnerBrakeRange(request);
        
        fakeit::Verify(Method(configRepositoryMock, saveUserSettings)).Once();
        fakeit::Verify(Method(processingThreadMock, reloadUserConfig)).Once();
    }

    SECTION("setOuterBrakeRange() => deactivated field is missing")
    {
        auto data = correctOuterBrakeConfiguration;
        data.erase("deactivated");

        auto request = new Request("test", data);

        try {
            controller->setOuterBrakeRange(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field deactivated");
        }
    }

    SECTION("setOuterBrakeRange() => min field is missing")
    {
        auto data = correctOuterBrakeConfiguration;
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
        auto data = correctOuterBrakeConfiguration;
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
        auto data = correctOuterBrakeConfiguration;
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
        auto data = correctOuterBrakeConfiguration;
        data["max"] = "abc";

        auto request = new Request("test", data);

        try {
            controller->setOuterBrakeRange(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field max is not a number");
        }
    }

    SECTION("setOuterBrakeRange() => deactivated field not a boolean")
    {
        auto data = correctOuterBrakeConfiguration;
        data["deactivated"] = "abc";

        auto request = new Request("test", data);

        try {
            controller->setOuterBrakeRange(request);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field deactivated is not boolean");
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
                {AccelerationStage(500, 1250)}, AccelerationMode::targetSpeed, false, false));

        fakeit::When(Method(configRepositoryMock, saveUserSettings)).AlwaysDo([] (std::string name, UserSettings* settings) {
            CHECK(name == "default");
            CHECK(settings->getInnerBrakeRange()->getMin() == 1000);
            CHECK(settings->getInnerBrakeRange()->getMax() == 2000);
            CHECK(settings->getOuterBrakeRange()->getMin() == 1673);
            CHECK(settings->getOuterBrakeRange()->getMax() == 4434);
            CHECK(settings->isOuterBrakeDeactivated());
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
            CHECK(settings->isOuterBrakeDeactivated());
            CHECK(settings->getSoftStartSpeed() == 100);
            CHECK(settings->getSoftStartAcceleration() == 1000);
            CHECK(settings->getAccelerationStages().size() == 1);
            CHECK(settings->getAccelerationStages().front().getSpeed() == 500);
            CHECK(settings->getAccelerationStages().front().getAcceleration() == 1250);
        });

        auto request = new Request("test", correctOuterBrakeConfiguration);
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
                {AccelerationStage(500, 1250)}, AccelerationMode::targetSpeed, false, true));

        fakeit::When(Method(configRepositoryMock, saveUserSettings)).AlwaysDo([] (std::string name, UserSettings* settings) {
            CHECK(name == "default");
            CHECK(settings->getInnerBrakeRange()->getMin() == 1000);
            CHECK(settings->getInnerBrakeRange()->getMax() == 2000);
            CHECK(settings->getOuterBrakeRange()->getMin() == 3000);
            CHECK(settings->getOuterBrakeRange()->getMax() == 4000);
            CHECK(settings->isOuterBrakeDeactivated());
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
            CHECK(settings->isOuterBrakeDeactivated());
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
                {AccelerationStage(500, 1250)}, AccelerationMode::targetSpeed, false, false));

        fakeit::When(Method(configRepositoryMock, saveUserSettings)).AlwaysDo([] (std::string name, UserSettings* settings) {
            CHECK(name == "default");
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
                {AccelerationStage(500, 1250)}, AccelerationMode::targetSpeed, false, true));

        fakeit::When(Method(configRepositoryMock, saveUserSettings)).AlwaysDo([] (std::string name, UserSettings* settings) {
            CHECK(name == "default");
            CHECK(settings->getInnerBrakeRange()->getMin() == 1000);
            CHECK(settings->getInnerBrakeRange()->getMax() == 2000);
            CHECK(settings->getOuterBrakeRange()->getMin() == 3000);
            CHECK(settings->getOuterBrakeRange()->getMax() == 4000);
            CHECK(settings->isOuterBrakeDeactivated());
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
            CHECK(settings->isOuterBrakeDeactivated());
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
                {AccelerationStage(500, 1250)}, AccelerationMode::targetSpeed, false, true));

        fakeit::When(Method(configRepositoryMock, saveUserSettings)).AlwaysDo([] (std::string name, UserSettings* settings) {
            CHECK(name == "default");
            CHECK(settings->getInnerBrakeRange()->getMin() == 1000);
            CHECK(settings->getInnerBrakeRange()->getMax() == 2000);
            CHECK(settings->getOuterBrakeRange()->getMin() == 3000);
            CHECK(settings->getOuterBrakeRange()->getMax() == 4000);
            CHECK(settings->isOuterBrakeDeactivated());
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
            CHECK(settings->isOuterBrakeDeactivated());
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