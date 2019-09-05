//
// Created by marius on 25.07.19.
//

#ifndef PEGASUS_CONTROLLER_ASSERTION_HPP
#define PEGASUS_CONTROLLER_ASSERTION_HPP

#include "../../../nlohmann_json/json.hpp"

namespace Pegasus::Networking
{

class Assertion
{
    private:
        static void jsonKeyExists(const nlohmann::json& data, const std::string& key);

    public:
        static void jsonExistsAndNumber(const nlohmann::json& data, const std::string& key);
        static void jsonExistsAndObject(const nlohmann::json& data, const std::string& key);
};

}

#endif //PEGASUS_CONTROLLER_ASSERTION_HPP
