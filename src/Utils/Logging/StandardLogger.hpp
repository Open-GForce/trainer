#ifndef GFORCE_TRAINER_CONTROLLER_STANDARDLOGGER_HPP
#define GFORCE_TRAINER_CONTROLLER_STANDARDLOGGER_HPP

#include <string>
#include "LoggerInterface.hpp"

namespace GForce::Utils::Logging {

class StandardLogger : public LoggerInterface
{
    public:
        void error(std::string message) override;

        void info(std::string message) override;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_STANDARDLOGGER_HPP
