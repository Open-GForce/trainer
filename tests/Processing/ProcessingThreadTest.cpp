#include "../../../catch2/catch.hpp"
#include "../../../fake_it/single_header/catch/fakeit.hpp"
#include "../../src/Processing/ProcessingThread.hpp"

using namespace GForce::Processing;

TEST_CASE( "ProcessingThread tests", "[Processing]" )
{
    fakeit::Mock<ProcessingService> serviceMock;
    fakeit::Fake(Method(serviceMock, setFirstBrakeInput));
    fakeit::Fake(Method(serviceMock, setSecondBrakeInput));
    ProcessingService* service = &serviceMock.get();

    fakeit::Mock<BrakeInputThread> brakeThreadMock;
    fakeit::Fake(Method(brakeThreadMock, getFirstBrake));
    fakeit::Fake(Method(brakeThreadMock, getSecondBrake));
    BrakeInputThread* brakeThread = &brakeThreadMock.get();

    auto thread = new ProcessingThread(service);

    SECTION("Brake inputs sent to service")
    {
        fakeit::When(Method(brakeThreadMock, getFirstBrake)).Return(4381);
        fakeit::When(Method(brakeThreadMock, getSecondBrake)).AlwaysDo([thread] () {
            thread->stop();
            return 1064;
        });

        thread->start(brakeThread);

        fakeit::Verify(Method(serviceMock, setFirstBrakeInput)).Once();
        fakeit::Verify(Method(serviceMock, setFirstBrakeInput).Using(4381));

        fakeit::Verify(Method(serviceMock, setSecondBrakeInput)).Once();
        fakeit::Verify(Method(serviceMock, setSecondBrakeInput).Using(1064));
    }
}