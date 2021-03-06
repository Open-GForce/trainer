#ifndef GFORCE_TRAINER_CONTROLLER_OPERATIONSCONTROLLER_HPP
#define GFORCE_TRAINER_CONTROLLER_OPERATIONSCONTROLLER_HPP

#include "../../Processing/ProcessingThread.hpp"
#include "../Websocket/Request.hpp"
#include "HeartbeatResponse.hpp"

using namespace GForce::Processing;
using namespace GForce::API::Websocket;

namespace GForce::API::Controller {

class OperationsController
{
    private:
        ProcessingThread* processingThread{};

    public:
        explicit OperationsController(ProcessingThread *processingThread);

        /**
        * Ownership of request stays at caller
        */
        virtual void handleSpeedLimit(Request* request);
        virtual void handleRotationDirection(Request* request);
        virtual void handleReleaseStatus(Request* request);
        virtual void handleOperationMode(Request* request);
        virtual HeartbeatResponse* handleHeartbeat();

};

}

#endif //GFORCE_TRAINER_CONTROLLER_OPERATIONSCONTROLLER_HPP
