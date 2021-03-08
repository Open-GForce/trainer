#include <catch2/catch.hpp>
#include <fakeit.hpp>
#include "../../../src/Processing/BrakeInput/BrakeInputReceiveThread.hpp"
#include "../../../src/Processing/BrakeInput/BrakeInputMessage.hpp"
#include "../../../src/Utils/Logging/NullLogger.hpp"

using namespace GForce::Processing;
using namespace GForce::Processing::BrakeInput;

TEST_CASE( "BrakeInputReceiveThread tests", "[Processing]" )
{
    fakeit::Mock<TCPSocketInterface> socketMock;
    fakeit::Fake(Method(socketMock, read));
    fakeit::Fake(Method(socketMock, close));
    TCPSocketInterface* socket = &socketMock.get();

    fakeit::Mock<TCPSocketFactory> socketFactoryMock;
    fakeit::Fake(Method(socketFactoryMock, listen));
    TCPSocketFactory* socketFactory = &socketFactoryMock.get();

    auto thread = new BrakeInputReceiveThread(new NullLogger());
    thread->setSocket(socket);
    thread->setFactory(socketFactory);
    thread->setFailureThreshold(3);

    SECTION("Correct input values")
    {
        fakeit::When(Method(socketMock, read)).AlwaysDo([thread] () {
            thread->stop();
            return BrakeInputMessage(1793, 20494).toJSON().dump();
        });

        thread->start();
        CHECK(thread->getFirstBrake() == 1793);
        CHECK(thread->getSecondBrake() == 20494);
    }

    SECTION("Message count incremented")
    {
        fakeit::When(Method(socketMock, read)).AlwaysDo([thread] () {
            thread->stop();
            return BrakeInputMessage(1793, 20494).toJSON().dump();
        });

        CHECK(thread->getMessageCount() == 0);
        thread->start();
        CHECK(thread->getMessageCount() == 1);
    }

    SECTION("Socket is rebuild in case of exceeded failure threshold")
    {
        fakeit::Mock<TCPSocketInterface> secondSocketMock;
        fakeit::Fake(Method(secondSocketMock, read));
        fakeit::Fake(Method(secondSocketMock, close));
        TCPSocketInterface* secondSocket = &secondSocketMock.get();

        fakeit::When(Method(socketMock, read)).AlwaysDo([] () -> std::string {
            throw std::runtime_error("test");
        });

        fakeit::When(Method(socketFactoryMock, listen)).Return(secondSocket);

        fakeit::When(Method(secondSocketMock, read)).AlwaysDo([thread] () {
            thread->stop();
            return BrakeInputMessage(1793, 20494).toJSON().dump();
        });

        thread->start();
        fakeit::Verify(Method(socketMock, read)).Exactly(3);
        fakeit::Verify(Method(socketMock, close)).Once();
        fakeit::Verify(Method(socketFactoryMock, listen)).Once();
        fakeit::Verify(Method(secondSocketMock, read)).Once();
        CHECK(thread->getFirstBrake() == 1793);
        CHECK(thread->getSecondBrake() == 20494);
    }

    SECTION("Failure count is reset on successful loop")
    {
        int callCount = 0;

        fakeit::When(Method(socketMock, read)).AlwaysDo([thread, &callCount] () -> std::string {
            callCount++;

            switch (callCount) {
                case 1:
                case 2:
                    throw std::runtime_error("first failure");
                case 3:
                    return BrakeInputMessage(1793, 20494).toJSON().dump();
                case 4:
                case 5:
                    throw std::runtime_error("second failure");
                default:
                    thread->stop();
                    return BrakeInputMessage(8494, 48411).toJSON().dump();
            }
        });


        thread->start();
        fakeit::Verify(Method(socketMock, read)).Exactly(6);
        fakeit::Verify(Method(socketFactoryMock, listen)).Never();
        CHECK(thread->getFirstBrake() == 8494);
        CHECK(thread->getSecondBrake() == 48411);
    }
}