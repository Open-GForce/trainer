#include "AssertionFailedException.hpp"

using namespace GForce::Utils::Assertions;

AssertionFailedException::AssertionFailedException(const std::string& message)
{
    this->message = message;
}

std::string AssertionFailedException::getMessage()
{
    return this->message;
}

const char *AssertionFailedException::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT
{
    return this->message.c_str();
}
