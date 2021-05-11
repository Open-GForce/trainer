#ifndef GFORCE_TRAINER_CONTROLLER_CONFIGURATIONCONTROLLER_HPP
#define GFORCE_TRAINER_CONTROLLER_CONFIGURATIONCONTROLLER_HPP

#include "../../Processing/ProcessingThread.hpp"
#include "../../Configuration/ConfigRepository.hpp"
#include "../Websocket/Request.hpp"
#include "../../Configuration/ActiveConfigurationResponse.hpp"
#include "../../Configuration/AvailableUserSettingsResponse.hpp"

using namespace GForce::Processing;
using namespace GForce::API::Websocket;
using namespace GForce::Configuration;
using namespace GForce::Utils::Logging;

namespace GForce::API::Controller {

class ConfigurationController
{
    private:
        LoggerInterface* logger;
        ProcessingThread* processingThread;
        ConfigRepository* configRepository;

        /**
         * Name of current active settings
         */
        std::string currentSettingsName;

        /**
         * Validates the request and builds a Range object
         */
        static Range* buildRange(Request* request);

        /**
         * Saves the configuration and deletes the objects afterwards
         */
        void saveConfig(std::string name, UserSettings* newConfig, UserSettings* oldConfig);

    public:
        ConfigurationController(ProcessingThread *processingThread, ConfigRepository *configRepository, LoggerInterface *logger);

        /**
         * Loads the current user settings
         * Ownership of returned object moves to caller
         */
        virtual UserSettings* getUserSettings(Request *request);

        /**
         * Duplicates the default configuration and saves it with the given name
         */
        virtual void createUserSettings(Request* request);

        /**
        * Returns the name of the current active user settings
        * Ownership of returned object moves to caller
        */
        virtual ActiveConfigurationResponse* getActiveConfigurationName();

        /**
        * Returns a list of existing user settings names
        * Ownership of returned object moves to caller
        */
        virtual AvailableUserSettingsResponse* getAvailableUserSettings();

        /**
         * Loads and activates settings by given name
         */
        virtual void switchUserSettings(Request* request);

        /**
        * Deletes the given user settings by name
        */
        virtual void deleteUserSettings(Request* request);

        /**
        * Loads the current system settings
        * Ownership of returned object moves to caller
        */
        virtual SystemSettings* getSystemSettings();

        /**
         * Saving brake range configuration
         */
        virtual void setInnerBrakeRange(Request *request);
        virtual void setOuterBrakeRange(Request *request);

        virtual void setSoftStart(Request* request);
        virtual void setAccelerationStages(Request* request);

        virtual void setUserInterfaceSettings(Request* request);
};

}

#endif //GFORCE_TRAINER_CONTROLLER_CONFIGURATIONCONTROLLER_HPP
