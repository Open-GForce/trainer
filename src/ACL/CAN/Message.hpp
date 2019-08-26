#ifndef GFORCE_TRAINER_CONTROLLER_MESSAGE_HPP
#define GFORCE_TRAINER_CONTROLLER_MESSAGE_HPP

#include <vector>
#include <string>
#include <sstream>
#include <cstdint>

namespace GForce::ACL::CAN {

class Message
{
    private:
        int index;
        std::vector<uint8_t> words;

        static std::string intToHex(int value);
    public:
        Message(int index, const std::vector<uint8_t>& words);

        std::string toString();
};

}

#endif //GFORCE_TRAINER_CONTROLLER_MESSAGE_HPP
