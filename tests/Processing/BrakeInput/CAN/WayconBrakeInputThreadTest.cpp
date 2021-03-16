#include <catch2/catch.hpp>
#include <fakeit.hpp>
#include "../../../../src/Processing/BrakeInput/CAN/WayconBrakeInputThread.hpp"
#include "../../../../src/Utils/Logging/NullLogger.hpp"
#include "../../../../src/ACL/CAN/Message.hpp"

using namespace GForce::Processing;
using namespace GForce::Processing::BrakeInput::CAN;

TEST_CASE( "WayconBrakeInputThread tests", "[Processing BrakeInput CAN Waycon]" )
{
    fakeit::Mock<CANThread> canThreadMock;
    fakeit::Fake(Method(canThreadMock, send));
    fakeit::Fake(Method(canThreadMock, getFirstBrakeMessages));
    fakeit::Fake(Method(canThreadMock, getSecondBrakeMessages));
    CANThread* canThread = &canThreadMock.get();

    auto thread = new WayconBrakeInputThread(canThread, new NullLogger());

    SECTION("Sensors started")
    {
        bool firstSensorStarted = false;
        bool secondSensorStarted = false;

        fakeit::When(Method(canThreadMock, send)).AlwaysDo([&firstSensorStarted, &secondSensorStarted] (MessageInterface* message) {
            REQUIRE(message != nullptr);

            CHECK(message->getIndex() == 0x0);
            CHECK(message->getData().size() == 2);
            CHECK(message->getData()[0] == 0x1);

            if (message->getData()[1] == WayconBrakeInputThread::NODE_FIRST_SENSOR) {
                firstSensorStarted = true;
                return;
            }

            if (message->getData()[1] == WayconBrakeInputThread::NODE_SECOND_SENSOR) {
                secondSensorStarted = true;
                return;
            }

            FAIL("Unexpected node started");
        });
        fakeit::When(Method(canThreadMock, getSecondBrakeMessages)).AlwaysReturn({});

        thread->stop();
        thread->start();

        CHECK(firstSensorStarted);
        CHECK(secondSensorStarted);
    }

    SECTION("No messages arrived")
    {
        fakeit::When(Method(canThreadMock, getFirstBrakeMessages)).AlwaysReturn({});
        fakeit::When(Method(canThreadMock, getSecondBrakeMessages)).AlwaysReturn({});

        thread->loop();

        CHECK(thread->getFirstBrake() == 0);
        CHECK(thread->getSecondBrake() == 0);
        CHECK(thread->getMessageCount() == 0);
    }

    SECTION("Correct value decoded for first brake")
    {
        fakeit::When(Method(canThreadMock, getFirstBrakeMessages)).AlwaysReturn({
            new Message(0x187, {0x01, 0x02}),
            new Message(0x187, {0xF6, 0x2B})
        });
        fakeit::When(Method(canThreadMock, getSecondBrakeMessages)).AlwaysReturn({});

        thread->loop();

        CHECK(thread->getFirstBrake() == 11254);
        CHECK(thread->getSecondBrake() == 0);
        CHECK(thread->getMessageCount() == 0);
    }

    SECTION("Correct value decoded for second brake")
    {
        fakeit::When(Method(canThreadMock, getFirstBrakeMessages)).AlwaysReturn({});
        fakeit::When(Method(canThreadMock, getSecondBrakeMessages)).AlwaysReturn({
            new Message(0x188, {0x01, 0x02}),
            new Message(0x188, {0xB5, 0x12})
        });

        thread->loop();

        CHECK(thread->getFirstBrake() == 0);
        CHECK(thread->getSecondBrake() == 4789);
        CHECK(thread->getMessageCount() == 0);
    }

    SECTION("Message counts only incremented when message for both brakes received")
    {
        fakeit::When(Method(canThreadMock, getFirstBrakeMessages)).AlwaysReturn({});
        fakeit::When(Method(canThreadMock, getSecondBrakeMessages)).AlwaysReturn({
             new Message(0x188, {0x1, 0x2})
        });

        thread->loop();
        CHECK(thread->getMessageCount() == 0);

        fakeit::When(Method(canThreadMock, getFirstBrakeMessages)).AlwaysReturn({
            new Message(0x187, {0x1, 0x2})
        });
        fakeit::When(Method(canThreadMock, getSecondBrakeMessages)).AlwaysReturn({});

        thread->loop();
        CHECK(thread->getMessageCount() == 1);
    }
}