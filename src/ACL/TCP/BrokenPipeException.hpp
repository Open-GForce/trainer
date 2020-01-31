#ifndef GFORCE_TRAINER_CONTROLLER_BROKENPIPEEXCEPTION_HPP
#define GFORCE_TRAINER_CONTROLLER_BROKENPIPEEXCEPTION_HPP

#include "../../Utils/Exceptions/RuntimeException.hpp"

namespace GForce::ACL::TCP {

class BrokenPipeException : public GForce::Utils::Exceptions::RuntimeException
{
    public:
        BrokenPipeException(const std::string &message, int code) : RuntimeException(message, code) {}
};

}

#endif //GFORCE_TRAINER_CONTROLLER_BROKENPIPEEXCEPTION_HPP
