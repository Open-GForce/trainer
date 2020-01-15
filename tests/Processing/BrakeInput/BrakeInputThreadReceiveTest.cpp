#include "../../../includes/catch2/catch.hpp"
#include "../../../includes/fake_it/single_header/catch/fakeit.hpp"
#include "../../../src/Processing/BrakeInput/BrakeInputReceiveThread.hpp"
#include "../../../src/Processing/BrakeInput/BrakeInputMessage.hpp"
#include "../../../src/Utils/Logging/NullLogger.hpp"

using namespace GForce::Processing;
using namespace GForce::Processing::BrakeInput;

TEST_CASE( "BrakeInputReceiveThread tests", "[Processing]" )
{
    fakeit::Mock<TCPSocketInterface> socketMock;
    fakeit::Fake(Method(socketMock, read));
    TCPSocketInterface* socket = &socketMock.get();

    auto thread = new BrakeInputReceiveThread(new NullLogger());
    thread->setSocket(socket);

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
}