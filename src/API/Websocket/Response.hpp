#ifndef GFORCE_TRAINER_CONTROLLER_RESPONSETELEGRAM_HPP
#define GFORCE_TRAINER_CONTROLLER_RESPONSE_HPP

#include <string>
#include <nlohmann/json.hpp>

namespace GForce::API::Websocket {

class Response
{
    private:
        std::string type;
        nlohmann::json data;

    public:
        Response(std::string type, nlohmann::json data);

        nlohmann::json toJSON();

        const std::string &getType() const;
        const nlohmann::json &getData() const;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_RESPONSETELEGRAM_HPP
