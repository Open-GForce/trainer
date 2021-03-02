#include <catch2/catch.hpp>
#include "../../../src/Processing/BrakeInput/BrakeInputMessage.hpp"
#include "../../../src/Utils/Assertions/AssertionFailedException.hpp"

using namespace GForce::Processing::BrakeInput;
using namespace GForce::Utils::Assertions;

TEST_CASE( "BrakeInputMessage tests", "[Processing BrakeInput]" )
{
    SECTION("Correct JSON decoding")
    {
        auto correctJSON = (new BrakeInputMessage(7674, 60191))->toJSON();

        auto message = BrakeInputMessage::fromJSON(correctJSON);
        CHECK(message.getFirstBrake() == 7674);
        CHECK(message.getSecondBrake() == 60191);
    }

    SECTION("JSON decoding => firstBrake missing")
    {
        auto json = (new BrakeInputMessage(7674, 60191))->toJSON();
        json.erase("firstBrake");

        try {
            BrakeInputMessage::fromJSON(json);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field firstBrake");
        }
    }

    SECTION("JSON decoding => secondBrake missing")
    {
        auto json = (new BrakeInputMessage(7674, 60191))->toJSON();
        json.erase("secondBrake");

        try {
            BrakeInputMessage::fromJSON(json);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. Missing JSON field secondBrake");
        }
    }

    SECTION("JSON decoding => firstBrake not Number")
    {
        auto json = (new BrakeInputMessage(7674, 60191))->toJSON();
        json["firstBrake"] = "abc";

        try {
            BrakeInputMessage::fromJSON(json);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field firstBrake is not a number");
        }
    }

    SECTION("JSON decoding => secondBrake not Number")
    {
        auto json = (new BrakeInputMessage(7674, 60191))->toJSON();
        json["secondBrake"] = "abc";

        try {
            BrakeInputMessage::fromJSON(json);
            FAIL("Expected exception was not thrown");
        } catch (AssertionFailedException &e) {
            CHECK(e.getMessage() == "Assertion failed. JSON field secondBrake is not a number");
        }
    }

    SECTION("Correct JSON encoding")
    {
        auto message = new BrakeInputMessage(164, 50494);
        CHECK(message->toJSON().dump() == "{\"firstBrake\":164,\"secondBrake\":50494}");
    }
}