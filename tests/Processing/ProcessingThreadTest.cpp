#include <catch2/catch.hpp>
#include <fakeit.hpp>
#include "../../src/Processing/ProcessingThread.hpp"
#include "../../src/Utils/Logging/NullLogger.hpp"

using namespace GForce::Processing;
using namespace GForce::Processing::BrakeInput;
using namespace GForce::API;
using namespace GForce::Utils::Logging;

TEST_CASE( "ProcessingThread tests", "[Processing]" )
{
    fakeit::Mock<ProcessingService> serviceMock;
    fakeit::Fake(Method(serviceMock, setFirstBrakeInput));
    fakeit::Fake(Method(serviceMock, setSecondBrakeInput));
    fakeit::Fake(Method(serviceMock, init));
    fakeit::Fake(Method(serviceMock, run));
    ProcessingService* service = &serviceMock.get();

    fakeit::Mock<BrakeInputReceiveThread> brakeThreadMock;
    fakeit::Fake(Method(brakeThreadMock, getFirstBrake));
    fakeit::Fake(Method(brakeThreadMock, getSecondBrake));
    fakeit::Fake(Method(brakeThreadMock, getMessageCount));
    BrakeInputReceiveThread* brakeThread = &brakeThreadMock.get();

    fakeit::Mock<Websocket::ServerThread> serverThreadMock;
    fakeit::Fake(Method(serverThreadMock, addBroadcastMessage));
    Websocket::ServerThread* serverThread = &serverThreadMock.get();

    auto thread = new ProcessingThread(new NullLogger(), service);

    SECTION("Brake inputs sent to service")
    {
        fakeit::When(Method(serviceMock, getStatus)).Return(nullptr);

        fakeit::When(Method(brakeThreadMock, getMessageCount)).Return(0);
        fakeit::When(Method(brakeThreadMock, getFirstBrake)).Return(4381);
        fakeit::When(Method(brakeThreadMock, getSecondBrake)).AlwaysDo([thread] () {
            thread->stop();
            return 1064;
        });

        thread->start(brakeThread, serverThread);

        fakeit::Verify(Method(serviceMock, init)).Once();
        fakeit::Verify(Method(serviceMock, run)).Once();

        fakeit::Verify(Method(serviceMock, setFirstBrakeInput)).Once();
        fakeit::Verify(Method(serviceMock, setFirstBrakeInput).Using(4381));

        fakeit::Verify(Method(serviceMock, setSecondBrakeInput)).Once();
        fakeit::Verify(Method(serviceMock, setSecondBrakeInput).Using(1064));
    }

    SECTION("Processing status sent to websocket thread based on status interval")
    {
        auto status = new ProcessingStatus(new EngineStatus(true, false, true, 1, 2, false, true, false), 1497, 3500,1450, 1434, 1000, 0.44, 0.35, RotationDirection::right,"regularSpiral");
        int cycleCount = 0;

        fakeit::When(Method(serviceMock, getStatus)).AlwaysReturn(status);

        fakeit::When(Method(brakeThreadMock, getMessageCount)).AlwaysReturn(0);
        fakeit::When(Method(brakeThreadMock, getFirstBrake)).AlwaysReturn(4381);
        fakeit::When(Method(brakeThreadMock, getSecondBrake)).AlwaysDo([thread, &cycleCount] () {
            cycleCount++;
            if (cycleCount >= 3) {
                thread->stop();
            }
            return 1064;
        });

        fakeit::When(Method(serverThreadMock, addBroadcastMessage)).AlwaysDo([status, &cycleCount] (Websocket::ResponseCastInterface* message) {
            REQUIRE(message != nullptr);
            CHECK(message == status);
            CHECK(cycleCount == 3);
        });

        thread->setStatusInterval(3);
        thread->start(brakeThread, serverThread);

        fakeit::Verify(Method(serverThreadMock, addBroadcastMessage)).Once();
    }


    SECTION("Correct timeout logic")
    {
        fakeit::When(Method(serviceMock, getStatus)).AlwaysReturn(nullptr);

        int cycleCount = 0;

        fakeit::When(Method(brakeThreadMock, getMessageCount)).AlwaysDo([thread, &cycleCount] () {
            cycleCount++;

            if (cycleCount > 80) {
                thread->stop();
            }

            return cycleCount > 60 ? cycleCount : 0;
        });
        fakeit::When(Method(brakeThreadMock, getFirstBrake)).AlwaysReturn(4381);
        fakeit::When(Method(brakeThreadMock, getSecondBrake)).AlwaysReturn(7167);

        fakeit::When(Method(serviceMock, setFirstBrakeInput)).AlwaysDo([thread, &cycleCount] (int value) {
            if (cycleCount > 60) {
                if (value != 4381) {
                    FAIL("Expected recovered input on cycle " + std::to_string(cycleCount));
                }
            } else if (cycleCount >= 50) {
                if (value != 0) {
                    FAIL("Expected zero input on cycle " + std::to_string(cycleCount));
                }
            } else {
                if (value != 4381) {
                    FAIL("Expected not yet timeout on cycle " + std::to_string(cycleCount));
                }
            }
        });

        fakeit::When(Method(serviceMock, setSecondBrakeInput)).AlwaysDo([thread, &cycleCount] (int value) {
            if (cycleCount > 60) {
                if (value != 7167) {
                    FAIL("Expected recovered input on cycle " + std::to_string(cycleCount));
                }
            } else if (cycleCount >= 50) {
                if (value != 0) {
                    FAIL("Expected zero input on cycle " + std::to_string(cycleCount));
                }
            } else {
                if (value != 7167) {
                    FAIL("Expected not yet timeout on cycle " + std::to_string(cycleCount));
                }
            }
        });

        thread->start(brakeThread, serverThread);
    }
}