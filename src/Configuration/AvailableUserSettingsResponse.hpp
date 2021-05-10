#ifndef GFORCE_TRAINER_CONTROLLER_AVAILABLEUSERSETTINGSRESPONSE_HPP
#define GFORCE_TRAINER_CONTROLLER_AVAILABLEUSERSETTINGSRESPONSE_HPP

#include <string>
#include <list>

namespace GForce::Configuration {

class AvailableUserSettingsResponse : public ResponseCastInterface
{
    private:
        std::list<std::string> names;

    public:
        AvailableUserSettingsResponse(const std::list<std::string> &names) : names(names) {}

        Response *toResponse() override
        {
            return new Response("availableUserSettings", {
                {"names", this->names}
            });
        }

        const std::list<std::string> &getNames() const {
            return names;
        }
};

}

#endif //GFORCE_TRAINER_CONTROLLER_AVAILABLEUSERSETTINGSRESPONSE_HPP
