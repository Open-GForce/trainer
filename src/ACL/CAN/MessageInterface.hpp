#ifndef GFORCE_TRAINER_CONTROLLER_MESSAGEINTERFACE_HPP
#define GFORCE_TRAINER_CONTROLLER_MESSAGEINTERFACE_HPP

#include <string>

namespace GForce::ACL::CAN {

class MessageInterface
{
    public:
        virtual ~MessageInterface() = default;

        virtual std::string toString() = 0;
        virtual int getIndex() = 0;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_MESSAGEINTERFACE_HPP
