#include <catch2/catch.hpp>
#include <fakeit.hpp>
#include "../../src/ACL/CAN/Message.hpp"
#include "../../src/Networking/CANThread.hpp"
#include "../../src/Utils/Logging/NullLogger.hpp"

using namespace GForce::ACL;
using namespace GForce::Networking;
using namespace GForce::Utils::Logging;

TEST_CASE( "CANThread tests", "[Networking]" )
{
    fakeit::Mock<CAN::SocketInterface> socketMock;
    fakeit::Fake(Method(socketMock, receive));
    fakeit::When(Method(socketMock, receive)).AlwaysReturn({});

    CAN::SocketInterface *socket = &socketMock.get();

    auto thread = new CANThread(socket, new NullLogger());

    SECTION("Foreign messages get discarded")
    {
        fakeit::When(Method(socketMock, receive)).AlwaysReturn({
            new Message(0x99, {})
        });

        thread->loop();

        CHECK(thread->getMoviDriveMessages().empty());
        CHECK(thread->getFirstBrakeMessages().empty());
        CHECK(thread->getSecondBrakeMessages().empty());
    }

    SECTION("Queue length kept within limits")
    {
        std::vector<MessageInterface*> messages = {
            new Message(CANThread::INDEX_MOVIDRIVE, {0x46})
        };

        for (uint8_t i = 0; i < 15; i++) {
            messages.push_back(new Message(CANThread::INDEX_BRAKE_SENSOR_1, {i}));
        }

        fakeit::When(Method(socketMock, receive)).AlwaysReturn(messages);

        thread->loop();

        auto moviMessages = thread->getMoviDriveMessages();
        auto brakeMessages = thread->getFirstBrakeMessages();

        REQUIRE(moviMessages.size() == 1);
        REQUIRE(moviMessages.front() != nullptr);
        REQUIRE(moviMessages.front()->getData().size() == 1);
        CHECK(moviMessages.front()->getData()[0] == 0x46);

        REQUIRE(brakeMessages.size() == 10);
        REQUIRE(brakeMessages.front() != nullptr);
        REQUIRE(brakeMessages.front()->getData().size() == 1);
        CHECK(brakeMessages.front()->getData()[0] == 5);

        REQUIRE(brakeMessages.back() != nullptr);
        REQUIRE(brakeMessages.back()->getData().size() == 1);
        CHECK(brakeMessages.back()->getData()[0] == 14);

        CHECK(thread->getSecondBrakeMessages().empty());
    }

    SECTION("Queue cleared on fetch")
    {
        std::vector<MessageInterface*> messages = {
                new Message(CANThread::INDEX_MOVIDRIVE, {0x46}),
                new Message(CANThread::INDEX_BRAKE_SENSOR_1, {0x2}),
                new Message(CANThread::INDEX_BRAKE_SENSOR_2, {0x3}),
        };

        fakeit::When(Method(socketMock, receive)).AlwaysReturn(messages);

        thread->loop();

        thread->getMoviDriveMessages();
        auto moviMessages = thread->getMoviDriveMessages();

        auto firstBrakeMessages = thread->getFirstBrakeMessages();
        auto secondBrakeMessages = thread->getSecondBrakeMessages();

        CHECK(moviMessages.empty());
        CHECK(!firstBrakeMessages.empty());
        CHECK(!secondBrakeMessages.empty());
    }

    SECTION("Correct queue content")
    {
        std::vector<MessageInterface*> messages = {
                new Message(CANThread::INDEX_MOVIDRIVE, {0x1}),
                new Message(CANThread::INDEX_BRAKE_SENSOR_1, {0x2}),
                new Message(CANThread::INDEX_BRAKE_SENSOR_2, {0x3}),
                new Message(CANThread::INDEX_MOVIDRIVE, {0x4}),
                new Message(CANThread::INDEX_BRAKE_SENSOR_1, {0x5}),
                new Message(CANThread::INDEX_BRAKE_SENSOR_2, {0x6}),
        };

        fakeit::When(Method(socketMock, receive)).AlwaysReturn(messages);

        thread->loop();

        auto moviMessages = thread->getMoviDriveMessages();
        auto firstBrakeMessages = thread->getFirstBrakeMessages();
        auto secondBrakeMessages = thread->getSecondBrakeMessages();

        REQUIRE(moviMessages.size() == 2);
        REQUIRE(firstBrakeMessages.size() == 2);
        REQUIRE(secondBrakeMessages.size() == 2);

        REQUIRE(moviMessages.front() != nullptr);
        REQUIRE(moviMessages.front()->getData().size() == 1);
        CHECK(moviMessages.front()->getData()[0] == 0x1);

        REQUIRE(moviMessages.back() != nullptr);
        REQUIRE(moviMessages.back()->getData().size() == 1);
        CHECK(moviMessages.back()->getData()[0] == 0x4);

        REQUIRE(firstBrakeMessages.front() != nullptr);
        REQUIRE(firstBrakeMessages.front()->getData().size() == 1);
        CHECK(firstBrakeMessages.front()->getData()[0] == 0x2);

        REQUIRE(firstBrakeMessages.back() != nullptr);
        REQUIRE(firstBrakeMessages.back()->getData().size() == 1);
        CHECK(firstBrakeMessages.back()->getData()[0] == 0x5);

        REQUIRE(secondBrakeMessages.front() != nullptr);
        REQUIRE(secondBrakeMessages.front()->getData().size() == 1);
        CHECK(secondBrakeMessages.front()->getData()[0] == 0x3);

        REQUIRE(secondBrakeMessages.back() != nullptr);
        REQUIRE(secondBrakeMessages.back()->getData().size() == 1);
        CHECK(secondBrakeMessages.back()->getData()[0] == 0x6);
    }
}