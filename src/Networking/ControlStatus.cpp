#include "ControlStatus.hpp"

using namespace GForce::Networking;

ControlStatus::ControlStatus(bool softBrakeActivated, bool hardBrakeActivated, bool holdingBrakeActivated, bool holdControlActivated): softBrakeActivated(softBrakeActivated), hardBrakeActivated(hardBrakeActivated),holdingBrakeActivated(holdingBrakeActivated), holdControlActivated(holdControlActivated) {}

uint16_t ControlStatus::toBinary()
{
    uint16_t value = 0;

    if (holdControlActivated) {
        value |= 1 << 3;
    }

    if (!softBrakeActivated) {
        value |= 1 << 2;
    }

    if (!hardBrakeActivated) {
        value |= 1 << 1;
    }

    if (holdingBrakeActivated) {
        value |= 1 << 0;
    }

    return value;
}

ControlStatus* ControlStatus::release() {
    return new ControlStatus(false, false, false, false);
}

ControlStatus* ControlStatus::softBrake() {
    return new ControlStatus(true, false, false, false);
}

ControlStatus* ControlStatus::hardBrake() {
    return new ControlStatus(false, true, false, false);
}

ControlStatus* ControlStatus::holdingBrake() {
    return new ControlStatus(false, false, true, false);
}

ControlStatus* ControlStatus::holdingControl() {
    return new ControlStatus(false, false, false, true);
}

bool ControlStatus::isSoftBrakeActivated() const {
    return softBrakeActivated;
}

nlohmann::json ControlStatus::toJson()
{
    nlohmann::json data = {
            {"softBrakeActivated", softBrakeActivated},
            {"hardBrakeActivated", hardBrakeActivated},
            {"holdingBrakeActivated", holdingBrakeActivated},
            {"holdControlActivated", holdControlActivated},
    };

    return data;
}
