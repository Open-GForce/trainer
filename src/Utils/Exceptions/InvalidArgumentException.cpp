#include "InvalidArgumentException.hpp"

using namespace GForce::Utils::Exceptions;

InvalidArgumentException::InvalidArgumentException(std::string message) : message(std::move(message)) {}

const char *InvalidArgumentException::what() const _GLIBCXX_USE_NOEXCEPT {
    return this->message.c_str();
}

std::string InvalidArgumentException::getMessage() {
    return this->message;
}
