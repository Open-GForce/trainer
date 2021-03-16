#include <catch2/catch.hpp>
#include <fakeit.hpp>
#include "../../src/ACL/CAN/Message.hpp"
#include "../../src/Networking/MoviDriveService.hpp"
#include "../../src/Utils/Logging/NullLogger.hpp"

using namespace GForce::ACL;
using namespace GForce::Networking;
using namespace GForce::Utils::Logging;

TEST_CASE( "MoviDrive service tests", "[Networking]" )
{
    fakeit::Mock<CANThread> canThreadMock;
    fakeit::Fake(Method(canThreadMock, send));
    fakeit::When(Method(canThreadMock, getMoviDriveMessages)).AlwaysReturn({});

    CANThread* canThread = &canThreadMock.get();

    auto service = new MoviDriveService(canThread, new NullLogger());

    SECTION("Correct message encoding of control status and positive speed + sync message")
    {
        /**
         * Speed conversion:
         * PDO resolution: 1 digit = 1/SPEED_SCALE_FACTOR/min
         * So 4500 1/min => 22500
         *
         * 22500 in hex => 57 E4
         * Low byte is sent first
         *
         * 10000 in hex => 27 10
         * Low byte is sent first
         */

        fakeit::When(Method(canThreadMock, send)).AlwaysDo([] (CAN::MessageInterface* message) {
            REQUIRE(message != nullptr);
            CHECK(message->toFrame() == "< send 202 6 06 00 92 71 10 27 >");
        });

        service->setControlStatus(ControlStatus::release());
        service->setRotationSpeed(4500);
        service->setAcceleration(10000);
        service->sync();

        fakeit::Verify(Method(canThreadMock, send)).Once();
    }

    SECTION("Correct message encoding of control status and negative speed + sync message")
    {
        /**
         * Speed conversion:
         * PDO resolution: 1 digit = 1/SPEED_SCALE_FACTOR/min
         * So -50 1/min => -8333
         *
         * PDO is using 16bit signed, so 65536 - 8333 => 57203
         * 57203 => 0xDF73
         * Low byte is sent first
         */

        fakeit::When(Method(canThreadMock, send)).AlwaysDo([] (CAN::MessageInterface* message) {
            REQUIRE(message != nullptr);
            CHECK(message->toFrame() == "< send 202 6 06 00 73 DF 00 00 >");
        });

        service->setControlStatus(ControlStatus::release());
        service->setRotationSpeed(-50);
        service->sync();

        fakeit::Verify(Method(canThreadMock, send)).Once();
    }

    SECTION("Correct response decoding => no messages => null returned")
    {
        fakeit::When(Method(canThreadMock, getMoviDriveMessages)).AlwaysReturn({});

        service->setControlStatus(ControlStatus::release());
        service->setRotationSpeed(0);
        auto response = service->sync();

        CHECK(response == nullptr);
    }

    SECTION("Correct response decoding => on error => null returned")
    {
        fakeit::When(Method(canThreadMock, getMoviDriveMessages)).AlwaysDo([] () {
            throw std::exception();

            std::list<CAN::MessageInterface*> messages = {};
            return messages;
        });

        service->setControlStatus(ControlStatus::release());
        service->setRotationSpeed(0);
        auto response = service->sync();

        CHECK(response == nullptr);
    }

    SECTION("Correct response decoding => only last message used")
    {
        fakeit::When(Method(canThreadMock, getMoviDriveMessages)).AlwaysReturn({
            new CAN::Message(0x182, {0x07, 0x04, 0x0, 0x0}),
            new CAN::Message(0x182, {0x07, 0x04, 0x1c, 0x03}),
        });

        service->setControlStatus(ControlStatus::release());
        service->setRotationSpeed(0);
        auto response = service->sync();

        REQUIRE(response != nullptr);
        CHECK(response->getRotationSpeed() == Approx(4.7762));
    }

    SECTION("Correct response decoding => only message with correct index used")
    {
        fakeit::When(Method(canThreadMock, getMoviDriveMessages)).AlwaysReturn({
            new CAN::Message(0x182, {0x07, 0x04, 0x0, 0x0}),
            new CAN::Message(0x182, {0x07, 0x04, 0x1c, 0x03}),
            new CAN::Message(0x666, {0x07, 0x04, 0x1c, 0x03}),
        });

        service->setControlStatus(ControlStatus::release());
        service->setRotationSpeed(0);
        auto response = service->sync();

        REQUIRE(response != nullptr);
        CHECK(response->getRotationSpeed() == Approx(4.7762));
    }

    SECTION("Correct response decoding => negative speed")
    {
        /**
         * 2^16 - 500 = 65036 => 0xFE0C
         * 500 / scale factor = ~ 3.0012
         */

        fakeit::When(Method(canThreadMock, getMoviDriveMessages)).AlwaysReturn({
            new CAN::Message(0x182, {0x07, 0x04, 0x0c, 0xFE})
        });

        service->setControlStatus(ControlStatus::release());
        service->setRotationSpeed(0);
        auto response = service->sync();

        REQUIRE(response != nullptr);
        CHECK(response->getRotationSpeed() == Approx(-3.000120005));
    }

    SECTION("Heartbeat sent")
    {
        int callCount = 0;

        fakeit::When(Method(canThreadMock, send)).AlwaysDo([&callCount] (CAN::MessageInterface* message) {
            callCount++;

            switch (callCount) {
                case 1:
                case 2:
                case 4:
                    CHECK(message->getIndex() == 0x202);
                    break;
                case 3:
                    CHECK(message->getIndex() == 0x702);
                    break;
                default:
                    FAIL("send() method called more then four times");
            }
        });

        service->setHeartbeatInterval(2);
        service->setControlStatus(ControlStatus::release());
        service->setRotationSpeed(0);

        for (int i = 0; i < 3; i++) {
            service->sync();
        }

        fakeit::Verify(Method(canThreadMock, send)).Exactly(4);
    }


    SECTION("Soft brake loop in case of errors")
    {
        int callCount = 0;

        fakeit::When(Method(canThreadMock, send)).AlwaysDo([&callCount] (CAN::MessageInterface* message) {
            callCount++;

            switch (callCount) {
                case 1:
                case 2:
                case 4:
                    CHECK(message->getIndex() == 0x202);
                    break;
                case 3:
                    CHECK(message->getIndex() == 0x702);
                    break;
                default:
                    FAIL("send() method called more then four times");
            }
        });

        service->setHeartbeatInterval(2);
        service->setControlStatus(ControlStatus::release());
        service->setRotationSpeed(0);

        for (int i = 0; i < 3; i++) {
            service->sync();
        }

        fakeit::Verify(Method(canThreadMock, send)).Exactly(4);
    }


    SECTION("Soft brake loop in case of errors")
    {
        int callCount = 0;

        fakeit::When(Method(canThreadMock, send)).AlwaysDo([&callCount, service] (CAN::MessageInterface* message) {
            callCount++;

            switch (callCount) {
                case 1:
                    throw std::exception();
                case 2:
                    throw std::exception();
                case 3:
                case 4:
                case 5:
                    CHECK(message->toFrame() == "< send 202 6 02 00 00 00 00 00 >"); // Soft break
                    break;
                case 6:
                    service->resetErrorCount();
                    break; // Error recovery
                default:
                    FAIL("send() method called more then six times. Call count: " + std::to_string(callCount));
            }
        });

        service->setErrorThreshold(2);
        service->setControlStatus(ControlStatus::release());
        service->setRotationSpeed(0);

        service->sync();
        service->sync();

        fakeit::Verify(Method(canThreadMock, send)).Exactly(6);
    }


    SECTION("Correct node start sequence")
    {
        int callCount = 0;

        fakeit::When(Method(canThreadMock, send)).AlwaysDo([&callCount, service] (CAN::MessageInterface* message) {
            callCount++;

            switch (callCount) {
                case 1:
                    CHECK(message->getIndex() == 0);
                    CHECK(message->getData().size() == 2);
                    CHECK(message->getData()[0] == 0x80);
                    CHECK(message->getData()[1] == 0x2);
                    break;
                case 2:
                    CHECK(message->getIndex() == 0);
                    CHECK(message->getData().size() == 2);
                    CHECK(message->getData()[0] == 0x1);
                    CHECK(message->getData()[1] == 0x2);
                    break;
                default:
                    FAIL("send() method called more then two times");
            }
        });

        service->startNode();

        fakeit::Verify(Method(canThreadMock, send)).Exactly(2);
    }
}