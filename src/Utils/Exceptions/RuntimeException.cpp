#include "RuntimeException.hpp"

using namespace GForce::Utils::Exceptions;

RuntimeException::RuntimeException(std::string message) : message(std::move(message)) {}

const char *RuntimeException::what() const _GLIBCXX_USE_NOEXCEPT {
    return this->message.c_str();
}

std::string RuntimeException::getMessage() {
    return this->message;
}
