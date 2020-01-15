#include "../../includes/catch2/catch.hpp"
#include "../../includes/fake_it/single_header/catch/fakeit.hpp"
#include "../../src/Networking/BusResponse.hpp"
#include "../../src/ACL/CAN/Message.hpp"

using namespace GForce::ACL;
using namespace GForce::Networking;

TEST_CASE( "MoviDrive response tests", "[Networking]" )
{
    SECTION("correct CAN message to Response decoding => amplifier not released")
    {
        auto message = new CAN::Message(0x182, {0x0, 0x0, 0x0, 0x0});
        auto response = BusResponse::fromMessage(message);

        CHECK(!response->getEngineStatus()->isPowerAmplifierReleased());
    }

    SECTION("correct CAN message to Response decoding => amplifier released")
    {
        auto message = new CAN::Message(0x182, {0x1, 0x0, 0x0, 0x0});
        auto response = BusResponse::fromMessage(message);

        CHECK(response->getEngineStatus()->isPowerAmplifierReleased());
    }

    SECTION("correct CAN message to Response decoding => inverter ready")
    {
        auto message = new CAN::Message(0x182, {0x2, 0x0, 0x0, 0x0});
        auto response = BusResponse::fromMessage(message);

        CHECK(response->getEngineStatus()->isInverterReady());
    }

    SECTION("correct CAN message to Response decoding => inverter not ready")
    {
        auto message = new CAN::Message(0x182, {0x0, 0x0, 0x0, 0x0});
        auto response = BusResponse::fromMessage(message);

        CHECK(!response->getEngineStatus()->isInverterReady());
    }

    SECTION("correct CAN message to Response decoding => input data ready")
    {
        auto message = new CAN::Message(0x182, {0x4, 0x0, 0x0, 0x0});
        auto response = BusResponse::fromMessage(message);

        CHECK(response->getEngineStatus()->isInputDataReady());
    }

    SECTION("correct CAN message to Response decoding => input data not ready")
    {
        auto message = new CAN::Message(0x182, {0x0, 0x0, 0x0, 0x0});
        auto response = BusResponse::fromMessage(message);

        CHECK(!response->getEngineStatus()->isInputDataReady());
    }

    SECTION("correct CAN message to Response decoding => integrator set 1")
    {
        auto message = new CAN::Message(0x182, {0x0, 0x0, 0x0, 0x0});
        auto response = BusResponse::fromMessage(message);

        CHECK(response->getEngineStatus()->getCurrentIntegratorSet() == 1);
    }

    SECTION("correct CAN message to Response decoding => integrator set 2")
    {
        auto message = new CAN::Message(0x182, {0x8, 0x0, 0x0, 0x0});
        auto response = BusResponse::fromMessage(message);

        CHECK(response->getEngineStatus()->getCurrentIntegratorSet() == 2);
    }

    SECTION("correct CAN message to Response decoding => parameter set 1")
    {
        auto message = new CAN::Message(0x182, {0x0, 0x0, 0x0, 0x0});
        auto response = BusResponse::fromMessage(message);

        CHECK(response->getEngineStatus()->getCurrentParameterSet() == 1);
    }

    SECTION("correct CAN message to Response decoding => parameter set 2")
    {
        auto message = new CAN::Message(0x182, {0x10, 0x0, 0x0, 0x0});
        auto response = BusResponse::fromMessage(message);

        CHECK(response->getEngineStatus()->getCurrentParameterSet() == 2);
    }

    SECTION("correct CAN message to Response decoding => no malfunction")
    {
        auto message = new CAN::Message(0x182, {0x0, 0x0, 0x0, 0x0});
        auto response = BusResponse::fromMessage(message);

        CHECK(!response->getEngineStatus()->isMalfunction());
    }

    SECTION("correct CAN message to Response decoding => malfunction")
    {
        auto message = new CAN::Message(0x182, {0x20, 0x0, 0x0, 0x0});
        auto response = BusResponse::fromMessage(message);

        CHECK(response->getEngineStatus()->isMalfunction());
    }

    SECTION("correct CAN message to Response decoding => inactive right switch")
    {
        auto message = new CAN::Message(0x182, {0x0, 0x0, 0x0, 0x0});
        auto response = BusResponse::fromMessage(message);

        CHECK(!response->getEngineStatus()->isSwitchRightActive());
    }

    SECTION("correct CAN message to Response decoding => active right switch")
    {
        auto message = new CAN::Message(0x182, {0x40, 0x0, 0x0, 0x0});
        auto response = BusResponse::fromMessage(message);

        CHECK(response->getEngineStatus()->isSwitchRightActive());
    }

    SECTION("correct CAN message to Response decoding => inactive left switch")
    {
        auto message = new CAN::Message(0x182, {0x0, 0x0, 0x0, 0x0});
        auto response = BusResponse::fromMessage(message);

        CHECK(!response->getEngineStatus()->isSwitchLeftActive());
    }

    SECTION("correct CAN message to Response decoding => active left switch")
    {
        auto message = new CAN::Message(0x182, {0x80, 0x0, 0x0, 0x0});
        auto response = BusResponse::fromMessage(message);

        CHECK(response->getEngineStatus()->isSwitchLeftActive());
    }

    SECTION("correct CAN message to Response decoding => correct speed")
    {
        auto message = new CAN::Message(0x182, {0x07, 0x04, 0x1c, 0x03});
        auto response = BusResponse::fromMessage(message);

        CHECK(response->getRotationSpeed() == Approx(4.7762));
    }
}