#ifndef GFORCE_TRAINER_CONTROLLER_REQUEST_HPP
#define GFORCE_TRAINER_CONTROLLER_REQUEST_HPP

#include <string>
#include <json.hpp>

namespace GForce::API::Websocket {

class Request
{
    private:
        std::string type;
        nlohmann::json data;

    public:
        Request(const std::string &type, const nlohmann::json &data);

        const std::string &getType() const;
        const nlohmann::json &getData() const;
};

}


#endif //GFORCE_TRAINER_CONTROLLER_REQUEST_HPP
