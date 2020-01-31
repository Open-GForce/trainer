#include "RuntimeException.hpp"

#include <utility>

using namespace GForce::Utils::Exceptions;

RuntimeException::RuntimeException(std::string message) : message(std::move(message)) {
    this->code = 0;
}

RuntimeException::RuntimeException(std::string message, int code) : message(std::move(message)), code(code) {}

const char *RuntimeException::what() const _GLIBCXX_USE_NOEXCEPT {
    return this->message.c_str();
}

std::string RuntimeException::getMessage() {
    return this->message;
}

int RuntimeException::getCode() const {
    return code;
}