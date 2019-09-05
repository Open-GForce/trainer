#include "Request.hpp"

using namespace GForce::API::Websocket;

Request::Request(const std::string &type, const nlohmann::json &data) : type(type), data(data) {}

const std::string &Request::getType() const {
    return type;
}

const nlohmann::json &Request::getData() const {
    return data;
}
