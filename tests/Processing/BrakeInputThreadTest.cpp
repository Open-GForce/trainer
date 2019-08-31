#include "../../../catch2/catch.hpp"
#include "../../../fake_it/single_header/catch/fakeit.hpp"
#include "../../src/Processing/BrakeInputThread.hpp"
#include "../../src/ACL/CAN/Message.hpp"
#include "../../src/Utils/Logging/NullLogger.hpp"

using namespace GForce::Processing;

TEST_CASE( "BrakeInputThread tests", "[Processing]" )
{
    fakeit::Mock<ADCSensorInterface> sensorMock;
    fakeit::Fake(Method(sensorMock, read));
    ADCSensorInterface* sensor = &sensorMock.get();

    auto thread = new BrakeInputThread(sensor, new NullLogger());

    SECTION("Correct ADC values")
    {
        int callCount = 0;

        fakeit::When(Method(sensorMock, read)).AlwaysDo([&callCount, thread] (int channel) {
            callCount++;

            switch (callCount) {
                case 1:
                    return (1793);
                case 2:
                    thread->stop();
                    return (32796);
                default:
                    FAIL("read() method called more then two times");
            }
        });

        thread->start();
        CHECK(thread->getFirstBrake() == 1793);
        CHECK(thread->getSecondBrake() == 32796);
    }

    SECTION("On exception all values set to Zero")
    {
        int callCount = 0;

        fakeit::When(Method(sensorMock, read)).AlwaysDo([&callCount, thread] (int channel) {
            callCount++;

            switch (callCount) {
                case 1:
                    return (1793);
                case 2:
                    thread->stop();
                    throw std::exception();
                default:
                    FAIL("read() method called more then two times");
            }
        });

        thread->start();
        CHECK(thread->getFirstBrake() == 0);
        CHECK(thread->getSecondBrake() == 0);
    }
}