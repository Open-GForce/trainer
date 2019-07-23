#include <utility>
#include "IOException.hpp"

using namespace GForce::ACL::I2C;

IOException::IOException(std::string message) : message(std::move(message)) {}

const char *IOException::what() const _GLIBCXX_USE_NOEXCEPT {
    return this->message.c_str();
}

std::string IOException::getMessage() {
    return nullptr;
}
