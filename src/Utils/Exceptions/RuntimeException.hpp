#ifndef GFORCE_TRAINER_CONTROLLER_RUNTIMEEXCEPTION_HPP
#define GFORCE_TRAINER_CONTROLLER_RUNTIMEEXCEPTION_HPP

#include <exception>
#include <string>

namespace GForce::Utils::Exceptions {

class RuntimeException : public std::exception
{
    private:
        int code;
        std::string message;

    public:
        explicit RuntimeException(std::string message);
        RuntimeException(std::string message, int code);

        [[nodiscard]] std::string getMessage();

        [[nodiscard]] const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT override;
        [[nodiscard]] int getCode() const;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_RUNTIMEEXCEPTION_HPP
