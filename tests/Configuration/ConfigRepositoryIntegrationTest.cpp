#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc30-c"

#include <fstream>
#include <boost/filesystem.hpp>
#include "../../includes/catch2/catch.hpp"
#include "../../includes/fake_it/single_header/catch/fakeit.hpp"
#include "../../src/Configuration/ConfigRepository.hpp"
#include "../../src/Utils/Exceptions/RuntimeException.hpp"

using namespace GForce::Configuration;
using namespace GForce::Utils::Exceptions;

TEST_CASE( "Configuration repository test", "[Configuration]" )
{
    std::string basePath = "/tmp/test_" + std::to_string(std::rand());
    boost::filesystem::create_directory(basePath);

    auto repository = new ConfigRepository(basePath);

    nlohmann::json correctConfig = {
            {"innerBrakeRange", {
               {"min", 1000},
               {"max", 2500},
            }},
            {"outerBrakeRange", {
               {"min", 500},
               {"max", 1400},
           }},
           {"rotationRadius", 7.6},
           {"softStartSpeed", 25},
           {"softStartAcceleration", 2500},
           {"accelerationStages", {}},
           {"useAdaptiveAccelerationUserInterface", true},
    };

    correctConfig["accelerationStages"].push_back({{"speed", 200}, {"acceleration", 5000}});
    correctConfig["accelerationStages"].push_back({{"speed", 300}, {"acceleration", 7500},});

    SECTION("Exception thrown if config file is missing")
    {
        try {
            repository->loadUserSettings();
            FAIL("Expected exception was not thrown");
        } catch (RuntimeException &e) {
            CHECK(e.getMessage() == "Config file " + basePath + "/user_settings.json does not exist");
        }
    }

    SECTION("Default values for range if section is missing")
    {
        nlohmann::json data = correctConfig;
        data.erase("innerBrakeRange");

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getInnerBrakeRange()->getMin() == 0);
        CHECK(config->getInnerBrakeRange()->getMax() == 10000);
    }

    SECTION("Default values for range if section sub key is missing")
    {
        nlohmann::json data = correctConfig;
        data["innerBrakeRange"].erase("min");

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getInnerBrakeRange()->getMin() == 0);
        CHECK(config->getInnerBrakeRange()->getMax() == 10000);
    }

    SECTION("Default values for range if section sub key is not number")
    {
        nlohmann::json data = correctConfig;
        data["innerBrakeRange"]["max"] = "abc";

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getInnerBrakeRange()->getMin() == 0);
        CHECK(config->getInnerBrakeRange()->getMax() == 10000);
    }

    SECTION("Default values for rotation radius if key is missing")
    {
        nlohmann::json data = correctConfig;
        data.erase("rotationRadius");

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getRotationRadius() == 3.87);
    }

    SECTION("Default values for rotation radius if value is not number")
    {
        nlohmann::json data = correctConfig;
        data["rotationRadius"] = "abc";

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getRotationRadius() == 3.87);
    }

    SECTION("Default values for soft start speed if key is missing")
    {
        nlohmann::json data = correctConfig;
        data.erase("softStartSpeed");

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getSoftStartSpeed() == 10);
    }

    SECTION("Default values for rotation radius if value is not number")
    {
        nlohmann::json data = correctConfig;
        data["softStartSpeed"] = "abc";

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getSoftStartSpeed() == 10);
    }


    SECTION("Default values for soft start speed if key is missing")
    {
        nlohmann::json data = correctConfig;
        data.erase("softStartAcceleration");

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getSoftStartSpeed() == 25);
    }

    SECTION("Default values for rotation radius if value is not number")
    {
        nlohmann::json data = correctConfig;
        data["softStartAcceleration"] = "abc";

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getSoftStartSpeed() == 25);
    }

    SECTION("Default values for adaptive acceleration UI toggle if value is missing")
    {
        nlohmann::json data = correctConfig;
        data.erase("useAdaptiveAccelerationUserInterface");

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(!config->isAdaptiveAccelerationUIActivated());
    }

    SECTION("Default values for adaptive acceleration UI toggle if value is not boolean")
    {
        nlohmann::json data = correctConfig;
        data["useAdaptiveAccelerationUserInterface"] = "abc";

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(!config->isAdaptiveAccelerationUIActivated());
    }

    SECTION("Default values for acceleration stages if key is missing")
    {
        nlohmann::json data = correctConfig;
        data.erase("accelerationStages");

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getAccelerationStages().size() == 1);
        CHECK(config->getAccelerationStages().front().getSpeed() == 0);
        CHECK(config->getAccelerationStages().front().getAcceleration() == 1000);
    }

    SECTION("Default values for acceleration stages if element is faulty => missing speed")
    {
        nlohmann::json data = correctConfig;
        data.erase("accelerationStages");

        data["accelerationStages"] = {};
        data["accelerationStages"].push_back({{"acceleration", 5000}});

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getAccelerationStages().size() == 1);
        CHECK(config->getAccelerationStages().front().getSpeed() == 0);
        CHECK(config->getAccelerationStages().front().getAcceleration() == 1000);
    }

    SECTION("Default values for acceleration stages if element is faulty => missing acceleration")
    {
        nlohmann::json data = correctConfig;
        data.erase("accelerationStages");

        data["accelerationStages"] = {};
        data["accelerationStages"].push_back({{"speed", 200}});

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getAccelerationStages().size() == 1);
        CHECK(config->getAccelerationStages().front().getSpeed() == 0);
        CHECK(config->getAccelerationStages().front().getAcceleration() == 1000);
    }

    SECTION("Default values for acceleration stages if element is faulty => speed not number")
    {
        nlohmann::json data = correctConfig;
        data.erase("accelerationStages");

        data["accelerationStages"] = {};
        data["accelerationStages"].push_back({{"speed", "abc"}, {"acceleration", 5000}});

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getAccelerationStages().size() == 1);
        CHECK(config->getAccelerationStages().front().getSpeed() == 0);
        CHECK(config->getAccelerationStages().front().getAcceleration() == 1000);
    }

    SECTION("Default values for acceleration stages if element is faulty => acceleraiton not number")
    {
        nlohmann::json data = correctConfig;
        data.erase("accelerationStages");

        data["accelerationStages"] = {};
        data["accelerationStages"].push_back({{"speed", 200}, {"acceleration", "abc"}});

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getAccelerationStages().size() == 1);
        CHECK(config->getAccelerationStages().front().getSpeed() == 0);
        CHECK(config->getAccelerationStages().front().getAcceleration() == 1000);
    }

    SECTION("Config correctly decoded")
    {
        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << correctConfig.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getInnerBrakeRange()->getMin() == 1000);
        CHECK(config->getInnerBrakeRange()->getMax() == 2500);
        CHECK(config->getOuterBrakeRange()->getMin() == 500);
        CHECK(config->getOuterBrakeRange()->getMax() == 1400);
        CHECK(config->getRotationRadius() == 7.6);
        CHECK(config->getSoftStartSpeed() == 25);
        CHECK(config->getSoftStartAcceleration() == 2500);
        CHECK(config->getAccelerationStages().size() == 2);
        CHECK(config->getAccelerationStages().front().getSpeed() == 200);
        CHECK(config->getAccelerationStages().front().getAcceleration() == 5000);
        CHECK(config->getAccelerationStages().back().getSpeed() == 300);
        CHECK(config->getAccelerationStages().back().getAcceleration() == 7500);
        CHECK(config->isAdaptiveAccelerationUIActivated());
    }

    SECTION("Config saved correctly")
    {
        auto saved = new UserSettings(new Range(150, 720), new Range(550, 3792), 5.2, 125, 1500, {
            AccelerationStage(500.5, 7400)
        }, true);
        repository->saveUserSettings(saved);

        auto loaded = repository->loadUserSettings();

        CHECK(loaded->getInnerBrakeRange()->getMin() == 150);
        CHECK(loaded->getInnerBrakeRange()->getMax() == 720);
        CHECK(loaded->getOuterBrakeRange()->getMin() == 550);
        CHECK(loaded->getOuterBrakeRange()->getMax() == 3792);
        CHECK(loaded->getRotationRadius() == 5.2);
        CHECK(loaded->getAccelerationStages().size() == 1);
        CHECK(loaded->getAccelerationStages().front().getSpeed() == 500.5);
        CHECK(loaded->getAccelerationStages().front().getAcceleration() == 7400);
        CHECK(loaded->isAdaptiveAccelerationUIActivated());
    }

    boost::filesystem::remove_all(basePath);
}

#pragma clang diagnostic pop