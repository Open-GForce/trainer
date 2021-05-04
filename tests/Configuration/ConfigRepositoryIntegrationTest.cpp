#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc30-c"

#include <fstream>
#include <boost/filesystem.hpp>
#include <catch2/catch.hpp>
#include <fakeit.hpp>
#include "../../src/Configuration/ConfigRepository.hpp"
#include "../../src/Utils/Exceptions/RuntimeException.hpp"

using namespace GForce::Configuration;
using namespace GForce::Utils::Exceptions;

TEST_CASE( "Configuration repository user settings test", "[Configuration]" )
{
    std::string basePath = "/tmp/test_" + std::to_string(std::rand());
    boost::filesystem::create_directory(basePath);

    auto repository = new ConfigRepository(basePath);

    nlohmann::json correctUserConfig = {
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
           {"accelerationMode", "differential"},
           {"useAdaptiveAccelerationUserInterface", true},
           {"outerBrakeDeactivated", true},
    };

    correctUserConfig["accelerationStages"].push_back({{"speed", 200}, {"acceleration", 5000}});
    correctUserConfig["accelerationStages"].push_back({{"speed", 300}, {"acceleration", 7500},});

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
        nlohmann::json data = correctUserConfig;
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
        nlohmann::json data = correctUserConfig;
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
        nlohmann::json data = correctUserConfig;
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
        nlohmann::json data = correctUserConfig;
        data.erase("rotationRadius");

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getRotationRadius() == 3.87);
    }

    SECTION("Default values for rotation radius if value is not number")
    {
        nlohmann::json data = correctUserConfig;
        data["rotationRadius"] = "abc";

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getRotationRadius() == 3.87);
    }

    SECTION("Default values for soft start speed if key is missing")
    {
        nlohmann::json data = correctUserConfig;
        data.erase("softStartSpeed");

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getSoftStartSpeed() == 10);
    }

    SECTION("Default values for rotation radius if value is not number")
    {
        nlohmann::json data = correctUserConfig;
        data["softStartSpeed"] = "abc";

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getSoftStartSpeed() == 10);
    }


    SECTION("Default values for soft start speed if key is missing")
    {
        nlohmann::json data = correctUserConfig;
        data.erase("softStartAcceleration");

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getSoftStartSpeed() == 25);
    }

    SECTION("Default values for rotation radius if value is not number")
    {
        nlohmann::json data = correctUserConfig;
        data["softStartAcceleration"] = "abc";

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getSoftStartSpeed() == 25);
    }

    SECTION("Default values for adaptive acceleration UI toggle if value is missing")
    {
        nlohmann::json data = correctUserConfig;
        data.erase("useAdaptiveAccelerationUserInterface");

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(!config->isAdaptiveAccelerationUIActivated());
    }

    SECTION("Default values for adaptive acceleration UI toggle if value is not boolean")
    {
        nlohmann::json data = correctUserConfig;
        data["useAdaptiveAccelerationUserInterface"] = "abc";

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(!config->isAdaptiveAccelerationUIActivated());
    }

    SECTION("Default values for acceleration mode if key is missing")
    {
        nlohmann::json data = correctUserConfig;
        data.erase("accelerationMode");

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(config->getAccelerationMode() == AccelerationMode::targetSpeed);
    }

    SECTION("Default values for outer brake toggle if value is missing")
    {
        nlohmann::json data = correctUserConfig;
        data.erase("outerBrakeDeactivated");

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(!config->isOuterBrakeDeactivated());
    }

    SECTION("Default values for adaptive acceleration UI toggle if value is not boolean")
    {
        nlohmann::json data = correctUserConfig;
        data["outerBrakeDeactivated"] = "abc";

        std::ofstream configFile(basePath + "/user_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadUserSettings();

        CHECK(!config->isOuterBrakeDeactivated());
    }

    SECTION("Default values for acceleration stages if key is missing")
    {
        nlohmann::json data = correctUserConfig;
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
        nlohmann::json data = correctUserConfig;
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
        nlohmann::json data = correctUserConfig;
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
        nlohmann::json data = correctUserConfig;
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

    SECTION("Default values for acceleration stages if element is faulty => acceleration not number")
    {
        nlohmann::json data = correctUserConfig;
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
        configFile << correctUserConfig.dump() << "\n";
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
        CHECK(config->getAccelerationMode() == AccelerationMode::differential);
        CHECK(config->isAdaptiveAccelerationUIActivated());
        CHECK(config->isOuterBrakeDeactivated());
    }

    SECTION("Config saved correctly")
    {
        auto saved = new UserSettings(new Range(150, 720), new Range(550, 3792), 5.2, 125, 1500, {
                AccelerationStage(500.5, 7400)
        }, AccelerationMode::differential, true, true);
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
        CHECK(loaded->getAccelerationMode() == AccelerationMode::differential);
        CHECK(loaded->isAdaptiveAccelerationUIActivated());
        CHECK(loaded->isOuterBrakeDeactivated());
    }

    boost::filesystem::remove_all(basePath);
}


TEST_CASE( "Configuration repository system settings test", "[Configuration]" )
{
    std::string basePath = "/tmp/test_" + std::to_string(std::rand());
    boost::filesystem::create_directory(basePath);

    auto repository = new ConfigRepository(basePath);

    nlohmann::json correctSystemSettings = {
            {"brakeSensorProtocol", "IPNetwork"},
            {"forceTable", {
               {1200, 1050},
               {2500, 1140},
               {7000, 2022}
            }}
    };

    SECTION("Exception thrown if config file is missing")
    {
        try {
            repository->loadSystemSettings();
            FAIL("Expected exception was not thrown");
        } catch (RuntimeException &e) {
            CHECK(e.getMessage() == "Config file " + basePath + "/system_settings.json does not exist");
        }
    }

    SECTION("Default values for brake sensor protocol if key is missing")
    {
        nlohmann::json data = correctSystemSettings;
        data.erase("brakeSensorProtocol");

        std::ofstream configFile(basePath + "/system_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadSystemSettings();

        CHECK(config->getBrakeSensorProtocol() == IPNetwork);
    }

    SECTION("Default values for brake sensor protocol if value is not string")
    {
        nlohmann::json data = correctSystemSettings;
        data["brakeSensorProtocol"] = {{}};

        std::ofstream configFile(basePath + "/system_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadSystemSettings();

        CHECK(config->getBrakeSensorProtocol() == IPNetwork);
    }

    SECTION("Correct value for brakeSensorProtocol => IPNetwork")
    {
        nlohmann::json data = correctSystemSettings;
        data["brakeSensorProtocol"] = "IPNetwork";

        std::ofstream configFile(basePath + "/system_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadSystemSettings();

        CHECK(config->getBrakeSensorProtocol() == IPNetwork);
    }

    SECTION("Correct value for brakeSensorProtocol => CANopen")
    {
        nlohmann::json data = correctSystemSettings;
        data["brakeSensorProtocol"] = "CANopen";

        std::ofstream configFile(basePath + "/system_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadSystemSettings();

        CHECK(config->getBrakeSensorProtocol() == CANopen);
    }

    SECTION("Default force table if key is missing")
    {
        nlohmann::json data = correctSystemSettings;
        data.erase("forceTable");

        std::ofstream configFile(basePath + "/system_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadSystemSettings();
        auto forceTable = config->getForceTable();

        CHECK(forceTable.size() == 15);
        CHECK(forceTable.find(1800) != forceTable.end());
        CHECK(forceTable[1800] == 1936);
        CHECK(forceTable.find(5500) != forceTable.end());
        CHECK(forceTable[5500] == 3307);
        CHECK(forceTable.find(7000) != forceTable.end());
        CHECK(forceTable[7000] == 3727);
    }

    SECTION("Default force table if not array")
    {
        nlohmann::json data = correctSystemSettings;
        data["forceTable"] = "abc";

        std::ofstream configFile(basePath + "/system_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadSystemSettings();
        auto forceTable = config->getForceTable();

        CHECK(forceTable.size() == 15);
        CHECK(forceTable.find(1800) != forceTable.end());
        CHECK(forceTable[1800] == 1936);
        CHECK(forceTable.find(5500) != forceTable.end());
        CHECK(forceTable[5500] == 3307);
        CHECK(forceTable.find(7000) != forceTable.end());
        CHECK(forceTable[7000] == 3727);
    }

    SECTION("Default force table if empty")
    {
        nlohmann::json data = correctSystemSettings;
        data["forceTable"] = {};

        std::ofstream configFile(basePath + "/system_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadSystemSettings();
        auto forceTable = config->getForceTable();

        CHECK(forceTable.size() == 15);
        CHECK(forceTable.find(1800) != forceTable.end());
        CHECK(forceTable[1800] == 1936);
        CHECK(forceTable.find(5500) != forceTable.end());
        CHECK(forceTable[5500] == 3307);
        CHECK(forceTable.find(7000) != forceTable.end());
        CHECK(forceTable[7000] == 3727);
    }

    SECTION("Correct values for force table decoded")
    {
        nlohmann::json data = correctSystemSettings;

        std::ofstream configFile(basePath + "/system_settings.json");
        configFile << data.dump() << "\n";
        configFile.close();

        auto config = repository->loadSystemSettings();
        auto forceTable = config->getForceTable();

        CHECK(forceTable.size() == 3);
        CHECK(forceTable.find(1200) != forceTable.end());
        CHECK(forceTable[1200] == 1050);
        CHECK(forceTable.find(2500) != forceTable.end());
        CHECK(forceTable[2500] == 1140);
        CHECK(forceTable.find(7000) != forceTable.end());
        CHECK(forceTable[7000] == 2022);
    }

    boost::filesystem::remove_all(basePath);
}

#pragma clang diagnostic pop