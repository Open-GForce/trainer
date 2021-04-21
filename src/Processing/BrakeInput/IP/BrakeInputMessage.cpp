#include "BrakeInputMessage.hpp"
#include "../../../Utils/Assertions/Assertion.hpp"

using namespace GForce::Processing::BrakeInput::IP;
using namespace GForce::Utils::Assertions;

BrakeInputMessage::BrakeInputMessage(int firstBrake, int secondBrake) : firstBrake(firstBrake), secondBrake(secondBrake) {}

BrakeInputMessage BrakeInputMessage::fromJSON(const nlohmann::json& data)
{
    Assertion::jsonExistsAndNumber(data, "firstBrake");
    Assertion::jsonExistsAndNumber(data, "secondBrake");

    return BrakeInputMessage(data["firstBrake"], data["secondBrake"]);
}

nlohmann::json BrakeInputMessage::toJSON()
{
    nlohmann::json data = {
            {"firstBrake", this->firstBrake},
            {"secondBrake", this->secondBrake}
    };

    return data;
}

int BrakeInputMessage::getFirstBrake() const {
    return firstBrake;
}

int BrakeInputMessage::getSecondBrake() const {
    return secondBrake;
}


