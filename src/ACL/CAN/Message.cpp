#include <utility>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "Message.hpp"
#include "../../Utils/Exceptions/InvalidArgumentException.hpp"

using namespace GForce::ACL::CAN;
using namespace GForce::Utils::Exceptions;

Message::Message(int index, const std::vector<uint8_t>& words) : index(index), words(words)
{
    if (index < 0 || index > 4095) {
        throw InvalidArgumentException("Given CAN ID " + std::to_string(index) + " is invalid (Valid 0-4095)");
    }

    if (words.size() > 8) {
        throw InvalidArgumentException("Maximum CAN message length of 8 bytes exceeded. Given message is " + std::to_string(words.size()) + " long");
    }
}

std::string Message::toString()
{
    std::string data;
    data += Message::intToHex(this->index) + "#";
    
    while (data.size() < 4) {
        data = "0" + data;
    }

    for (uint8_t word : this->words) {
        data += Message::intToHex(word);
    }

    return data;
}

std::string Message::intToHex(int value)
{
    std::stringstream stream;
    stream << std::hex << value;

    // Padding with 0 to grantee two digits
    std::string string = stream.str().size() == 1
                         ? ("0" + stream.str())
                         : stream.str();

    // Converting to uppercase
    std::transform(string.begin(), string.end(), string.begin(), ::toupper);

    return string;
}

int Message::getIndex() {
    return this->index;
}


