#include <utility>
#include <iostream>
#include "PigpioException.hpp"

using namespace GForce::ACL::Pigpio;

PigpioException::PigpioException(std::string message)
{
    this->message = std::move(message);
}

std::string PigpioException::getMessage()
{
    return this->message;
}

const char *PigpioException::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT
{
    return this->message.c_str();
}

