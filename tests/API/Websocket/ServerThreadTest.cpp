#include "../../../../catch2/catch.hpp"
#include "../../../../fake_it/single_header/catch/fakeit.hpp"
#include "../../../src/API/Websocket/ServerThread.hpp"
#include "../../../src/Utils/Logging/NullLogger.hpp"
#include "../../../src/Processing/ProcessingStatus.hpp"

using namespace GForce::API::Websocket;
using namespace GForce::Processing;
using namespace GForce::Utils::Logging;

TEST_CASE( "Websocket server thread tests", "[Processing]" )
{
    fakeit::Mock<Server> serverMock;
    fakeit::Fake(Method(serverMock, broadcast));
    Server* websocketServer = &serverMock.get();

    auto thread = new ServerThread(websocketServer, new NullLogger);

    SECTION("Broadcast message sent")
    {
        auto message = new ProcessingStatus(new EngineStatus(true, false, true, 1, 2, false, true, false), 1497, 3500, 1450, 1434, 1000, 0.44, 0.35, RotationDirection::right, "regularSpiral");
        thread->addBroadcastMessage(message);

        fakeit::When(Method(serverMock, broadcast)).AlwaysDo([thread] (Response* response) {
            thread->stop();
            CHECK(response->getType() == "processingStatus");
        });

        thread->start();
    }
}