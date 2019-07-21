#include <iostream>
#include <exception>

#ifndef ACL_PIGPIO_PIGPIO_EXCEPTION_H
#define ACL_PIGPIO_PIGPIO_EXCEPTION_H

namespace GForce {
namespace ACL {
namespace Pigpio {

    class PigpioException : public std::exception
    {
        protected:
            std::string message;

        public:
            PigpioException(std::string message);

            std::string getMessage();

        const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT override;
    };

}
}
}

#endif