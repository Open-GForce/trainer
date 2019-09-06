#include "../../../catch2/catch.hpp"
#include "../../../fake_it/single_header/catch/fakeit.hpp"
#include "../../src/ACL/CAN/Message.hpp"
#include "../../src/Networking/MoviDriveService.hpp"
#include "../../src/Utils/Logging/NullLogger.hpp"

using namespace GForce::ACL;
using namespace GForce::Networking;
using namespace GForce::Utils::Logging;

TEST_CASE( "MoviDrive service tests", "[Networking]" )
{
    fakeit::Mock<CAN::SocketInterface> socketMock;
    fakeit::Fake(Method(socketMock, send));
    fakeit::When(Method(socketMock, receive)).AlwaysReturn({});

    CAN::SocketInterface *socket = &socketMock.get();

    auto service = new MoviDriveService(socket, new NullLogger());

    SECTION("Correct message encoding of control status and positive speed + sync message")
    {
        /**
         * Speed conversion:
         * PDO resolution: 1 digit = 1/SPEED_SCALE_FACTOR/min
         * So 4500 1/min => 22500
         *
         * 22500 in hex => 57 E4
         * Low byte is sent first
         */

        int callCount = 0;

        fakeit::When(Method(socketMock, send)).AlwaysDo([&callCount] (CAN::MessageInterface* message) {
            callCount++;

            switch (callCount) {
                case 1:
                    CHECK(message->toFrame() == "< send 202 6 06 00 92 71 00 00 >");
                    break;
                case 2:
                    CHECK(message->toFrame() == "< send 080 0 >");
                    break;
                default:
                    FAIL("send() method called more then two times");
            }
        });

        service->setControlStatus(ControlStatus::release());
        service->setRotationSpeed(4500);
        service->sync();

        fakeit::Verify(Method(socketMock, send)).Exactly(2);
    }

    SECTION("Correct message encoding of control status and negative speed + sync message")
    {
        /**
         * Speed conversion:
         * PDO resolution: 1 digit = 1/SPEED_SCALE_FACTOR/min
         * So -1000 1/min => -5000
         *
         * PDO is using 16bit signed, so 65536 - 5000 => 60536
         * 60536 => 0xEC78
         * Low byte is sent first
         */

        int callCount = 0;

        fakeit::When(Method(socketMock, send)).AlwaysDo([&callCount] (CAN::MessageInterface* message) {
            callCount++;

            switch (callCount) {
                case 1:
                    CHECK(message->toFrame() == "< send 202 6 06 00 FC 74 00 00 >");
                    break;
                case 2:
                    CHECK(message->toFrame() == "< send 080 0 >");
                    break;
                default:
                    FAIL("send() method called more then two times");
            }
        });

        service->setControlStatus(ControlStatus::release());
        service->setRotationSpeed(-1000);
        service->sync();

        fakeit::Verify(Method(socketMock, send)).Exactly(2);
    }

    SECTION("Correct response decoding => no messages => null returned")
    {
        fakeit::When(Method(socketMock, receive)).AlwaysReturn({});

        service->setControlStatus(ControlStatus::release());
        service->setRotationSpeed(0);
        auto response = service->sync();

        CHECK(response == nullptr);
    }

    SECTION("Correct response decoding => on error => null returned")
    {
        fakeit::When(Method(socketMock, receive)).AlwaysDo([] () {
            throw std::exception();

            std::vector<CAN::MessageInterface*> messages = {};
            return messages;
        });

        service->setControlStatus(ControlStatus::release());
        service->setRotationSpeed(0);
        auto response = service->sync();

        CHECK(response == nullptr);
    }

    SECTION("Correct response decoding => only last message used")
    {
        fakeit::When(Method(socketMock, receive)).AlwaysReturn({
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
        fakeit::When(Method(socketMock, receive)).AlwaysReturn({
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

    SECTION("Heartbeat sent")
    {
        int callCount = 0;

        fakeit::When(Method(socketMock, send)).AlwaysDo([&callCount] (CAN::MessageInterface* message) {
            callCount++;

            switch (callCount) {
                case 1:
                case 3:
                case 6:
                    CHECK(message->getIndex() == 0x202);
                    break;
                case 2:
                case 4:
                case 7:
                    CHECK(message->getIndex() == 0x080);
                    break;
                case 5:
                    CHECK(message->getIndex() == 0x702);
                    break;
                default:
                    FAIL("send() method called more then seven times");
            }
        });

        service->setHeartbeatInterval(2);
        service->setControlStatus(ControlStatus::release());
        service->setRotationSpeed(0);

        for (int i = 0; i < 3; i++) {
            service->sync();
        }

        fakeit::Verify(Method(socketMock, send)).Exactly(7);
    }


    SECTION("Soft brake loop in case of errors")
    {
        int callCount = 0;

        fakeit::When(Method(socketMock, send)).AlwaysDo([&callCount] (CAN::MessageInterface* message) {
            callCount++;

            switch (callCount) {
                case 1:
                case 3:
                case 6:
                    CHECK(message->getIndex() == 0x202);
                    break;
                case 2:
                case 4:
                case 7:
                    CHECK(message->getIndex() == 0x080);
                    break;
                case 5:
                    CHECK(message->getIndex() == 0x702);
                    break;
                default:
                    FAIL("send() method called more then seven times");
            }
        });

        service->setHeartbeatInterval(2);
        service->setControlStatus(ControlStatus::release());
        service->setRotationSpeed(0);

        for (int i = 0; i < 3; i++) {
            service->sync();
        }

        fakeit::Verify(Method(socketMock, send)).Exactly(7);
    }


    SECTION("Soft brake loop in case of errors")
    {
        int callCount = 0;

        fakeit::When(Method(socketMock, send)).AlwaysDo([&callCount, service] (CAN::MessageInterface* message) {
            callCount++;

            switch (callCount) {
                case 1:
                    throw std::exception(); // PDO#1
                case 2:
                    break; // PDO#2
                case 3:
                    throw std::exception(); // Sync
                case 4:
                case 6:
                    CHECK(message->toFrame() == "< send 202 6 02 00 00 00 00 00 >"); // Soft break
                    break;
                case 5:
                case 7:
                    CHECK(message->toFrame() == "< send 080 0 >"); // Sync of soft break
                    break;
                case 8:
                    service->resetErrorCount(); // Second soft break
                case 9:
                    break; // Sync of second soft break
                default:
                    FAIL("send() method called more then nine times");
            }
        });

        service->setErrorThreshold(2);
        service->setControlStatus(ControlStatus::release());
        service->setRotationSpeed(0);

        service->sync();
        service->sync();

        fakeit::Verify(Method(socketMock, send)).Exactly(9);
    }
}