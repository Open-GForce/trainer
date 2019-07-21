#include <iostream>
#include <exception>
#include "PigpioException.hpp"

#ifndef ACL_PIGPIO_LOGIC_EXCEPTION_H
#define ACL_PIGPIO_LOGIC_EXCEPTION_H

namespace GForce {
namespace ACL {
namespace Pigpio {

    class LogicException : public PigpioException
    {
        using PigpioException::PigpioException;
    };

}
}
}

#endif