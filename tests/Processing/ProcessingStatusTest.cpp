#include <catch2/catch.hpp>
#include <fakeit.hpp>
#include "../../src/Processing/ProcessingStatus.hpp"

using namespace GForce::Processing;
using namespace GForce::Networking;

TEST_CASE( "ProcessingStatus tests", "[Processing]" )
{
    SECTION("correct response cast")
    {
        auto status = new ProcessingStatus(
                new EngineStatus(true, false, true, 1, 2, false, true, false),
                false,
                1497,
                3500,
                1450,
                1434,
                1000,
                0.44,
                0.35,
                RotationDirection::right,
                "regularSpiral");

        auto response = status->toResponse();

        CHECK(response->toJSON().dump() == "{\"data\":{\"engineStatus\":{\"currentIntegratorSet\":1,\"currentParameterSet\":2,\"inputDataReady\":true,\"inverterReady\":false,\"malfunction\":false,\"powerAmplifierReleased\":true,\"switchLeftActive\":false,\"switchRightActive\":true},\"innerBrake\":{\"raw\":1434,\"scaled\":0.44},\"maxSpeed\":3500.0,\"operationMode\":\"regularSpiral\",\"outerBrake\":{\"raw\":1000,\"scaled\":0.35},\"released\":false,\"rotationDirection\":\"right\",\"rotationSpeed\":1497.0,\"targetSpeed\":1450.0},\"type\":\"processingStatus\"}");
    }
}