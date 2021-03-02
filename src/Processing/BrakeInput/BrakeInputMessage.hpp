#ifndef GFORCE_TRAINER_CONTROLLER_BRAKEINPUTMESSAGE_HPP
#define GFORCE_TRAINER_CONTROLLER_BRAKEINPUTMESSAGE_HPP

#include <nlohmann/json.hpp>

namespace GForce::Processing::BrakeInput {

class BrakeInputMessage
{
    private:
        int firstBrake;
        int secondBrake;

    public:
        BrakeInputMessage(int firstBrake, int secondBrake);

        static BrakeInputMessage fromJSON(const nlohmann::json& data);
        nlohmann::json toJSON();

        [[nodiscard]] int getFirstBrake() const;
        [[nodiscard]] int getSecondBrake() const;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_BRAKEINPUTMESSAGE_HPP
