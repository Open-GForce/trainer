#include "../../../catch2/catch.hpp"
#include "../../../fake_it/single_header/catch/fakeit.hpp"
#include "../../src/Networking/MoviDriveService.hpp"
#include "../../src/Utils/Logging/NullLogger.hpp"

using namespace GForce::ACL;
using namespace GForce::Networking;
using namespace GForce::Utils::Logging;

TEST_CASE( "MoviDrive service tests", "[Networking]" )
{
    fakeit::Mock<CAN::SocketInterface> socketMock;
    fakeit::Fake(Method(socketMock, send));

    CAN::SocketInterface *socket = &socketMock.get();

    auto service = new MoviDriveService(socket, new NullLogger());

    SECTION("Correct message encoding of control status and speed + sync message")
    {
        int callCount = 0;

        fakeit::When(Method(socketMock, send)).AlwaysDo([&callCount] (CAN::MessageInterface* message) {
            callCount++;

            switch (callCount) {
                case 1:
                    CHECK(message->toString() == "182#06009411");
                    break;
                case 2:
                    CHECK(message->toString() == "080#");
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

    SECTION("Heartbeat sent")
    {
        int callCount = 0;

        fakeit::When(Method(socketMock, send)).AlwaysDo([&callCount] (CAN::MessageInterface* message) {
            callCount++;

            switch (callCount) {
                case 1:
                case 3:
                case 6:
                    CHECK(message->getIndex() == 0x182);
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
                    CHECK(message->getIndex() == 0x182);
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
                    CHECK(message->toString() == "182#02000000"); // Soft break
                    break;
                case 5:
                case 7:
                    CHECK(message->toString() == "080#"); // Sync of soft break
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