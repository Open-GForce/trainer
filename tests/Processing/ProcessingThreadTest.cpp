#include "../../../catch2/catch.hpp"
#include "../../../fake_it/single_header/catch/fakeit.hpp"
#include "../../src/Processing/ProcessingThread.hpp"

using namespace GForce::Processing;

TEST_CASE( "ProcessingThread tests", "[Processing]" )
{
    fakeit::Mock<ProcessingService> serviceMock;
    fakeit::Fake(Method(serviceMock, setFirstBrakeInput));
    fakeit::Fake(Method(serviceMock, setSecondBrakeInput));
    fakeit::Fake(Method(serviceMock, run));
    ProcessingService* service = &serviceMock.get();

    fakeit::Mock<BrakeInputThread> brakeThreadMock;
    fakeit::Fake(Method(brakeThreadMock, getFirstBrake));
    fakeit::Fake(Method(brakeThreadMock, getSecondBrake));
    BrakeInputThread* brakeThread = &brakeThreadMock.get();

    fakeit::Mock<ServerThread> serverThreadMock;
    fakeit::Fake(Method(serverThreadMock, addBroadcastMessage));
    ServerThread* serverThread = &serverThreadMock.get();

    auto thread = new ProcessingThread(service);

    SECTION("Brake inputs sent to service")
    {
        fakeit::When(Method(serviceMock, getStatus)).Return(nullptr);

        fakeit::When(Method(brakeThreadMock, getFirstBrake)).Return(4381);
        fakeit::When(Method(brakeThreadMock, getSecondBrake)).AlwaysDo([thread] () {
            thread->stop();
            return 1064;
        });

        thread->start(brakeThread, serverThread);

        fakeit::Verify(Method(serviceMock, run)).Once();

        fakeit::Verify(Method(serviceMock, setFirstBrakeInput)).Once();
        fakeit::Verify(Method(serviceMock, setFirstBrakeInput).Using(4381));

        fakeit::Verify(Method(serviceMock, setSecondBrakeInput)).Once();
        fakeit::Verify(Method(serviceMock, setSecondBrakeInput).Using(1064));
    }

    SECTION("Processing status sent to websocket thread")
    {
        auto status = new ProcessingStatus(new EngineStatus(true, false, true, 1, 2, false, true, false), 1497, 3500, 1450, 1434, 1000, 0.44, 0.35, RotationDirection::right);

        fakeit::When(Method(serviceMock, getStatus)).Return(status);

        fakeit::When(Method(brakeThreadMock, getFirstBrake)).Return(4381);
        fakeit::When(Method(brakeThreadMock, getSecondBrake)).AlwaysDo([thread] () {
            thread->stop();
            return 1064;
        });

        fakeit::When(Method(serverThreadMock, addBroadcastMessage)).AlwaysDo([status] (ResponseCastInterface* message) {
            REQUIRE(message != nullptr);
            CHECK(message == status);
        });

        thread->start(brakeThread, serverThread);

        fakeit::Verify(Method(serverThreadMock, addBroadcastMessage)).Once();
    }
}