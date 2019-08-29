#include "EngineStatus.hpp"

using namespace GForce::Networking;

EngineStatus::EngineStatus(bool powerAmplifierReleased, bool inverterReady, bool paDataReady,
                                               int currentIntegratorSet, int currentParameterSet, bool malfunction,
                                               bool switchRightActive, bool switchLeftActive) : powerAmplifierReleased(
        powerAmplifierReleased), inverterReady(inverterReady), inputDataReady(paDataReady), currentIntegratorSet(
        currentIntegratorSet), currentParameterSet(currentParameterSet), malfunction(malfunction), switchRightActive(
        switchRightActive), switchLeftActive(switchLeftActive) {}

bool EngineStatus::isPowerAmplifierReleased() const {
    return powerAmplifierReleased;
}

bool EngineStatus::isInverterReady() const {
    return inverterReady;
}

bool EngineStatus::isInputDataReady() const {
    return inputDataReady;
}

int EngineStatus::getCurrentIntegratorSet() const {
    return currentIntegratorSet;
}

int EngineStatus::getCurrentParameterSet() const {
    return currentParameterSet;
}

bool EngineStatus::isMalfunction() const {
    return malfunction;
}

bool EngineStatus::isSwitchRightActive() const {
    return switchRightActive;
}

bool EngineStatus::isSwitchLeftActive() const {
    return switchLeftActive;
}
