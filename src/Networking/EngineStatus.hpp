#ifndef GFORCE_TRAINER_CONTROLLER_ENGINESTATUS_HPP
#define GFORCE_TRAINER_CONTROLLER_ENGINESTATUS_HPP

#include <json.hpp>

namespace GForce::Networking
{

class EngineStatus
{
    private:
        bool powerAmplifierReleased;
        bool inverterReady;
        bool inputDataReady;
        int currentIntegratorSet;
        int currentParameterSet;
        bool malfunction;
        bool switchRightActive;
        bool switchLeftActive;

    public:
        EngineStatus(bool powerAmplifierReleased, bool inverterReady, bool paDataReady, int currentIntegratorSet,
                     int currentParameterSet, bool malfunction, bool switchRightActive, bool switchLeftActive);

        EngineStatus* clone();

        nlohmann::json toJSON();

        bool isPowerAmplifierReleased() const;
        bool isInverterReady() const;
        bool isInputDataReady() const;
        int getCurrentIntegratorSet() const;
        int getCurrentParameterSet() const;
        bool isMalfunction() const;
        bool isSwitchRightActive() const;
        bool isSwitchLeftActive() const;
};

}


#endif //GFORCE_TRAINER_CONTROLLER_ENGINESTATUS_HPP
