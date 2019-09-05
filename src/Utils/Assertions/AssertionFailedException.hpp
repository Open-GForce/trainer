#ifndef PEGASUS_CONTROLLER_ASSERTIONFAILEDEXCEPTION_HPP
#define PEGASUS_CONTROLLER_ASSERTIONFAILEDEXCEPTION_HPP

#include <exception>
#include <string>

namespace GForce::Utils::Assertions {

class AssertionFailedException: public std::exception
{
    protected:
        std::string message;

    public:
        explicit AssertionFailedException(std::string message);

        std::string getMessage();

        const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT override;
};

}

#endif //PEGASUS_CONTROLLER_ASSERTIONFAILEDEXCEPTION_HPP
