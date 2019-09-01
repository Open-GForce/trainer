#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc30-c"

#include <fstream>
#include <boost/filesystem.hpp>
#include "../../../catch2/catch.hpp"
#include "../../../fake_it/single_header/catch/fakeit.hpp"
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
    };

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
    }

    SECTION("Config saved correctly")
    {
        auto saved = new UserSettings(new Range(150, 720), new Range(550, 3792));
        repository->saveUserSettings(saved);

        auto loaded = repository->loadUserSettings();

        CHECK(loaded->getInnerBrakeRange()->getMin() == 150);
        CHECK(loaded->getInnerBrakeRange()->getMax() == 720);
        CHECK(loaded->getOuterBrakeRange()->getMin() == 550);
        CHECK(loaded->getOuterBrakeRange()->getMax() == 3792);
    }

    boost::filesystem::remove_all(basePath);
}

#pragma clang diagnostic pop