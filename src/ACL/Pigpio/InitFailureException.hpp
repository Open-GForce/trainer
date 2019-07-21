#include <iostream>
#include <exception>
#include "PigpioException.hpp"

#ifndef ACL_PIGPIO_INIT_FAILURE_EXCEPTION_H
#define ACL_PIGPIO_INIT_FAILURE_EXCEPTION_H

namespace GForce {
namespace ACL {
namespace Pigpio {

    class InitFailureException : public PigpioException
    {
        using PigpioException::PigpioException;
    };

}
}
}

#endif