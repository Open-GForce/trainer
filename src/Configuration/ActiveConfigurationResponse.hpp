#ifndef GFORCE_TRAINER_CONTROLLER_ACTIVECONFIGURATIONRESPONSE_HPP
#define GFORCE_TRAINER_CONTROLLER_ACTIVECONFIGURATIONRESPONSE_HPP

#include <string>

namespace GForce::Configuration {

class ActiveConfigurationResponse : public ResponseCastInterface
{
    private:
        std::string name;

    public:
        ActiveConfigurationResponse(const std::string &name) : name(name) {}

        Response *toResponse() override
        {
            return new Response("activeConfiguration", {
                {"name", this->name}
            });
        }

        const std::string &getName() const {
            return name;
        }
};

}

#endif //GFORCE_TRAINER_CONTROLLER_ACTIVECONFIGURATIONRESPONSE_HPP
