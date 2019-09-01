#include <utility>
#include "Response.hpp"

using namespace GForce::API::Websocket;

Response::Response(std::string type, nlohmann::json data) : type(std::move(type)), data(std::move(data)) {}

nlohmann::json Response::toJSON()
{
    nlohmann::json jsonData = {
        {"type", this->type},
        {"data", this->data},
    };

    return jsonData;
}

const std::string &Response::getType() const {
    return type;
}

const nlohmann::json &Response::getData() const {
    return data;
}
