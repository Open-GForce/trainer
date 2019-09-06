#include "../../../../catch2/catch.hpp"
#include "../../../../fake_it/single_header/catch/fakeit.hpp"
#include "../../../src/ACL/CAN/MessageFactory.hpp"

using namespace GForce::ACL::CAN;

TEST_CASE( "CAN message factory tests", "[ACL]" )
{
    SECTION("Correct decoding of message with multiple data bytes")
    {
        auto messages = MessageFactory::createFromString("< frame 072 23.424242 1100ef01 >");

        REQUIRE(messages.size() == 1);
        CHECK(messages[0]->getIndex() == 0x72);
        CHECK(messages[0]->getData().size() == 4);
        CHECK(messages[0]->getData()[0] == 0x11);
        CHECK(messages[0]->getData()[1] == 0x00);
        CHECK(messages[0]->getData()[2] == 0xef);
        CHECK(messages[0]->getData()[3] == 0x01);
    }

    SECTION("Incomplete Byte pair => ignored")
    {
        auto messages = MessageFactory::createFromString("< frame 072 23.424242 1100ef012 >");

        REQUIRE(messages.size() == 1);
        CHECK(messages[0]->getIndex() == 0x72);
        CHECK(messages[0]->getData().size() == 4);
        CHECK(messages[0]->getData()[0] == 0x11);
        CHECK(messages[0]->getData()[1] == 0x00);
        CHECK(messages[0]->getData()[2] == 0xef);
        CHECK(messages[0]->getData()[3] == 0x01);
    }

    SECTION("Correct decoding of empty message")
    {
        auto messages = MessageFactory::createFromString("< frame 072 23.424242 >");

        REQUIRE(messages.size() == 1);
        CHECK(messages[0]->getIndex() == 0x72);
        CHECK(messages[0]->getData().empty());
    }

    SECTION("Correct decoding of multiple messages")
    {
        auto messages = MessageFactory::createFromString("< frame 801 23.424242 11 >< frame 802 23.424242 22 >");

        REQUIRE(messages.size() == 2);
        CHECK(messages[0]->getIndex() == 0x801);
        CHECK(messages[0]->getData()[0] == 0x11);
        CHECK(messages[1]->getIndex() == 0x802);
        CHECK(messages[1]->getData()[0] == 0x22);
    }

    SECTION("Frame not closed => ignored")
    {
        auto messages = MessageFactory::createFromString("< frame 801 23.424242 11 < frame 802 23.424242 22 >");

        REQUIRE(messages.size() == 1);
        CHECK(messages[0]->getIndex() == 0x802);
        CHECK(messages[0]->getData()[0] == 0x22);
    }

    SECTION("Frame not opened => ignored")
    {
        auto messages = MessageFactory::createFromString(" frame 801 23.424242 11>< frame 802 23.424242 22 >");

        REQUIRE(messages.size() == 1);
        CHECK(messages[0]->getIndex() == 0x802);
        CHECK(messages[0]->getData()[0] == 0x22);
    }

    SECTION("wrong prefix => ignored")
    {
        auto messages = MessageFactory::createFromString("< wrong_prefix 801 23.424242 11>< frame 802 23.424242 22 >");

        REQUIRE(messages.size() == 1);
        CHECK(messages[0]->getIndex() == 0x802);
        CHECK(messages[0]->getData()[0] == 0x22);
    }
}