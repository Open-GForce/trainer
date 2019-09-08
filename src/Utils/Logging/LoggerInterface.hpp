#ifndef GFORCE_TRAINER_CONTROLLER_LOGGERINTERFACE_HPP
#define GFORCE_TRAINER_CONTROLLER_LOGGERINTERFACE_HPP

#include <string>

namespace GForce::Utils::Logging {

class LoggerInterface
{
    public:
        virtual void info(std::string message) = 0;
        virtual void error(std::string message) = 0;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_LOGGERINTERFACE_HPP
