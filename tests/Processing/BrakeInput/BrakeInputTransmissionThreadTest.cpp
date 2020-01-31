#include "../../../includes/catch2/catch.hpp"
#include "../../../includes/fake_it/single_header/catch/fakeit.hpp"
#include "../../../src/Processing/BrakeInput/BrakeInputTransmissionThread.hpp"
#include "../../../src/Utils/Logging/NullLogger.hpp"
#include "../../../src/ACL/TCP/TCPConnectionInterface.hpp"

using namespace GForce::Processing;
using namespace GForce::Processing::BrakeInput;

TEST_CASE( "BrakeInputTransmissionThread tests", "[Processing]" )
{
    fakeit::Mock<TCPConnectionInterface> socketMock;
    fakeit::Fake(Method(socketMock, send));
    TCPConnectionInterface* socket = &socketMock.get();

    fakeit::Mock<ADCSensorInterface> sensorMock;
    fakeit::Fake(Method(sensorMock, read));
    ADCSensorInterface* sensor = &sensorMock.get();

    auto thread = new BrakeInputTransmissionThread(new NullLogger(), sensor);
    thread->setSocket(socket);

    SECTION("Correct message sent")
    {
        fakeit::When(Method(sensorMock, read)).Return(1697, 60000);
        fakeit::When(Method(socketMock, send)).Do([thread] (const std::string& message) {
            thread->stop();
            CHECK(message == "{\"firstBrake\":1697,\"secondBrake\":-5536}");
        });

        thread->start();
        fakeit::Verify(Method(socketMock, send)).Once();
    }
}