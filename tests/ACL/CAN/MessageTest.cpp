#include "../../../includes/catch2/catch.hpp"
#include "../../../src/ACL/CAN/Message.hpp"
#include "../../../src/Utils/Exceptions/InvalidArgumentException.hpp"

using namespace GForce::ACL::CAN;
using namespace GForce::Utils::Exceptions;

TEST_CASE( "CAN message tests", "[ACL]" )
{
    SECTION("Negative index given => exception thrown")
    {
        try {
            new Message(-1, {});
            FAIL("No exception was thrown");
        } catch (InvalidArgumentException &e) {
            REQUIRE(e.getMessage() == "Given CAN ID -1 is invalid (Valid 0-4095)");
        }
    }

    SECTION("Too big CAN ID given => exception thrown")
    {
        try {
            new Message(5000, {});
            FAIL("No exception was thrown");
        } catch (InvalidArgumentException &e) {
            REQUIRE(e.getMessage() == "Given CAN ID 5000 is invalid (Valid 0-4095)");
        }
    }

    SECTION("Max size exceeded => exception thrown")
    {
        try {
            new Message(512, {1, 2, 3, 4, 5, 6, 7, 8, 9});
            FAIL("No exception was thrown");
        } catch (InvalidArgumentException &e) {
            REQUIRE(e.getMessage() == "Maximum CAN message length of 8 bytes exceeded. Given message is 9 long");
        }
    }

    SECTION("correct toFrame() => empty data")
    {
        auto message = new Message(800, {});
        CHECK(message->toFrame() == "< send 320 0 >");
    }

    SECTION("correct toFrame() => zero ID")
    {
        auto message = new Message(0, {});
        CHECK(message->toFrame() == "< send 000 0 >");
    }

    SECTION("correct toFrame() => single digit ID")
    {
        auto message = new Message(4, {});
        CHECK(message->toFrame() == "< send 004 0 >");
    }

    SECTION("correct toFrame() => two digit ID")
    {
        auto message = new Message(100, {});
        CHECK(message->toFrame() == "< send 064 0 >");
    }

    SECTION("correct toFrame() => zero data")
    {
        auto message = new Message(800, {0});
        CHECK(message->toFrame() == "< send 320 1 00 >");
    }

    SECTION("correct toFrame() => single Byte")
    {
        auto message = new Message(800, {0x12});
        CHECK(message->toFrame() == "< send 320 1 12 >");
    }

    SECTION("correct toFrame() => multiple bytes")
    {
        auto message = new Message(800, {0x12, 0x40, 0x00, 0xef});
        CHECK(message->toFrame() == "< send 320 4 12 40 00 EF >");
    }
}