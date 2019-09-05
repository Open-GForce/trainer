#include "AssertionFailedException.hpp"

using namespace Pegasus::Networking;

AssertionFailedException::AssertionFailedException(std::string message)
{
    this->message = std::move(message);
}

std::string AssertionFailedException::getMessage()
{
    return this->message;
}

const char *AssertionFailedException::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT
{
    return this->message.c_str();
}


