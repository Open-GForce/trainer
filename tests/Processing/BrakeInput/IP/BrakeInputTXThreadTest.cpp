#include <catch2/catch.hpp>
#include <fakeit.hpp>
#include "../../../../src/Processing/BrakeInput/IP/BrakeInputTXThread.hpp"
#include "../../../../src/Utils/Logging/NullLogger.hpp"
#include "../../../../src/ACL/TCP/TCPConnectionInterface.hpp"
#include "../../../../src/ACL/TCP/BrokenPipeException.hpp"


using namespace GForce::Processing;
using namespace GForce::Processing::BrakeInput;

TEST_CASE( "BrakeInputTransmissionThread tests", "[Processing]" )
{
    fakeit::Mock<TCPConnectionInterface> socketMock;
    fakeit::Fake(Method(socketMock, send));
    fakeit::Fake(Method(socketMock, close));
    TCPConnectionInterface* socket = &socketMock.get();

    fakeit::Mock<TCPConnectionFactory> socketFactoryMock;
    fakeit::Fake(Method(socketFactoryMock, connect));
    TCPConnectionFactory* socketFactory = &socketFactoryMock.get();

    fakeit::Mock<ADCSensorInterface> sensorMock;
    fakeit::Fake(Method(sensorMock, read));
    ADCSensorInterface* sensor = &sensorMock.get();

    auto thread = new IP::BrakeInputTXThread(new NullLogger(), sensor);
    thread->setSocket(socket);
    thread->setSocketFactory(socketFactory);

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

    SECTION("Reconnect in case of broken pipe")
    {
        fakeit::Mock<TCPConnectionInterface> secondSocketMock;
        fakeit::Fake(Method(secondSocketMock, send));
        fakeit::Fake(Method(secondSocketMock, close));
        TCPConnectionInterface* secondSocket = &secondSocketMock.get();

        // First socket throws broken pipe exception
        fakeit::When(Method(sensorMock, read)).Return(1, 2, 3, 4);
        fakeit::When(Method(socketMock, send)).Do([thread] (const std::string& message) {
            throw BrokenPipeException("test", 10);
        });

        // Factory creates new socket
        fakeit::When(Method(socketFactoryMock, connect)).Return(secondSocket);

        // Second sockets works
        fakeit::When(Method(secondSocketMock, send)).Do([thread] (const std::string& message) {
            thread->stop();
            CHECK(message == "{\"firstBrake\":2,\"secondBrake\":3}");
        });

        thread->start();
        fakeit::Verify(Method(socketMock, send)).Once();
        fakeit::Verify(Method(socketMock, close)).Once();
        fakeit::Verify(Method(socketFactoryMock, connect)).Once();
        fakeit::Verify(Method(secondSocketMock, send)).Once();
        fakeit::Verify(Method(secondSocketMock, close)).Never();
    }

    SECTION("No reconnect in case of other failure")
    {
        int callCount = 0;

        fakeit::When(Method(sensorMock, read)).Return(1, 2, 3, 4);
        fakeit::When(Method(socketMock, send)).AlwaysDo([thread, &callCount] (const std::string& message) {
            callCount++;

            if (callCount == 1) {
                throw std::runtime_error("test");
            }

            thread->stop();
            CHECK(message == "{\"firstBrake\":2,\"secondBrake\":3}");
        });

        thread->start();
        fakeit::Verify(Method(socketMock, send)).Exactly(2);
        fakeit::Verify(Method(socketMock, close)).Never();
        fakeit::Verify(Method(socketFactoryMock, connect)).Never();
    }
}