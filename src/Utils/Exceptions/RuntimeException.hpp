#ifndef GFORCE_TRAINER_CONTROLLER_INVALIDARGUMENTEXCEPTION_HPP
#define GFORCE_TRAINER_CONTROLLER_INVALIDARGUMENTEXCEPTION_HPP

#include <exception>
#include <string>

namespace GForce::Utils::Exceptions {

class RuntimeException : public std::exception
{
    protected:
        std::string message;

    public:
        explicit RuntimeException(std::string message);

        std::string getMessage();
        const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT override;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_INVALIDARGUMENTEXCEPTION_HPP
