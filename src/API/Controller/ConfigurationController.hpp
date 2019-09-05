#ifndef GFORCE_TRAINER_CONTROLLER_CONFIGURATIONCONTROLLER_HPP
#define GFORCE_TRAINER_CONTROLLER_CONFIGURATIONCONTROLLER_HPP

#include "../../Processing/ProcessingThread.hpp"
#include "../../Configuration/ConfigRepository.hpp"
#include "../Websocket/Request.hpp"

using namespace GForce::Processing;
using namespace GForce::API::Websocket;
using namespace GForce::Configuration;

namespace GForce::API::Controller {

class ConfigurationController
{
    private:
        ProcessingThread* processingThread;
        ConfigRepository* configRepository;

        /**
         * Validates the request and builds a Range object
         */
        static Range* buildRange(Request* request);

        /**
         * Saves the configuration and deletes the objects afterwards
         */
        void saveConfig(UserSettings* newConfig, UserSettings* oldConfig);

    public:
        ConfigurationController(ProcessingThread *processingThread, ConfigRepository *configRepository);

        /**
         * Saving brake range configuration
         */
        virtual void setInnerBrakeRange(Request *request);
        virtual void setOuterBrakeRange(Request *request);
};

}

#endif //GFORCE_TRAINER_CONTROLLER_CONFIGURATIONCONTROLLER_HPP
