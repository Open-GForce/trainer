#ifndef GFORCE_TRAINER_CONTROLLER_NULLLOGGER_HPP
#define GFORCE_TRAINER_CONTROLLER_NULLLOGGER_HPP

#include <string>
#include "LoggerInterface.hpp"

namespace GForce::Utils::Logging {

class NullLogger : public LoggerInterface
{
    public:
        void setGlobalContext(std::string key, std::string value) override {}

        void info(const std::string &channel, const std::string &message, const nlohmann::json &context) override {}
        void error(const std::string &channel, const std::string &message, const nlohmann::json &context) override {}
        void warning(const std::string &channel, const std::string &message, const nlohmann::json &context) override {}
};

}


#endif //GFORCE_TRAINER_CONTROLLER_NULLLOGGER_HPP
