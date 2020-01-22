#ifndef GFORCE_TRAINER_CONTROLLER_LOGGERINTERFACE_HPP
#define GFORCE_TRAINER_CONTROLLER_LOGGERINTERFACE_HPP

#include <string>
#include <json.hpp>

namespace GForce::Utils::Logging {

class LoggerInterface
{
    public:
        /**
         * Sets a global context field which gets logged with every message
         */
        virtual void setGlobalContext(std::string key, std::string value) = 0;

        virtual void info(const std::string &channel, const std::string &message, const nlohmann::json &context) = 0;
        virtual void error(const std::string &channel, const std::string &message, const nlohmann::json &context) = 0;
        virtual void warning(const std::string &channel, const std::string &message, const nlohmann::json &context) = 0;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_LOGGERINTERFACE_HPP
