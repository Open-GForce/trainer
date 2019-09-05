#ifndef GFORCE_TRAINER_CONTROLLER_OPERATIONSCONTROLLER_HPP
#define GFORCE_TRAINER_CONTROLLER_OPERATIONSCONTROLLER_HPP

#include "../../Processing/ProcessingThread.hpp"
#include "../Websocket/Request.hpp"

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
         * Ownership of request stays at owner
         */
        virtual void handleSpeedLimit(Request* request);
};

}

#endif //GFORCE_TRAINER_CONTROLLER_OPERATIONSCONTROLLER_HPP
