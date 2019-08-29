#ifndef GFORCE_TRAINER_CONTROLLER_NULLLOGGER_HPP
#define GFORCE_TRAINER_CONTROLLER_NULLLOGGER_HPP

#include <string>
#include "LoggerInterface.hpp"

namespace GForce::Utils::Logging {

class NullLogger : public LoggerInterface
{
    public:
        void error(std::string message) override {}
};

}


#endif //GFORCE_TRAINER_CONTROLLER_NULLLOGGER_HPP
