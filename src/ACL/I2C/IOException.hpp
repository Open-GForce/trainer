#ifndef GFORCE_TRAINER_CONTROLLER_IOEXCEPTION_HPP
#define GFORCE_TRAINER_CONTROLLER_IOEXCEPTION_HPP

#include <exception>
#include <string>

namespace GForce::ACL::I2C
{

class IOException : public std::exception
{
    protected:
        std::string message;

    public:
        explicit IOException(std::string message);

        std::string getMessage();
        const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT override;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_IOEXCEPTION_HPP
