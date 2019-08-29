#ifndef GFORCE_TRAINER_CONTROLLER_LOGGERINTERFACE_HPP
#define GFORCE_TRAINER_CONTROLLER_LOGGERINTERFACE_HPP

namespace GForce::Utils::Logging {

class LoggerInterface
{
    public:
        virtual void error(std::string message) = 0;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_LOGGERINTERFACE_HPP
