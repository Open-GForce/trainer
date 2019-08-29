#ifndef GFORCE_TRAINER_CONTROLLER_MESSAGE_HPP
#define GFORCE_TRAINER_CONTROLLER_MESSAGE_HPP

#include <vector>
#include <string>
#include <sstream>
#include <cstdint>
#include "MessageInterface.hpp"

namespace GForce::ACL::CAN {

class Message : public MessageInterface
{
    private:
        int index;
        std::vector<uint8_t> words;

        static std::string intToHex(int value);
    public:
        Message(int index, const std::vector<uint8_t>& words);

        std::string toFrame() override;

        int getIndex() override;
        int getSize() override;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_MESSAGE_HPP
