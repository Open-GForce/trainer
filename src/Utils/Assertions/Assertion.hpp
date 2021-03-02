#ifndef PEGASUS_CONTROLLER_ASSERTION_HPP
#define PEGASUS_CONTROLLER_ASSERTION_HPP

#include <nlohmann/json.hpp>

namespace GForce::Utils::Assertions
{

class Assertion
{
    private:
        static void jsonKeyExists(const nlohmann::json& data, const std::string& key);

    public:
        static void jsonExistsAndNumber(const nlohmann::json& data, const std::string& key);
        static void jsonExistsAndString(const nlohmann::json& data, const std::string& key);
        static void jsonExistsAndBool(const nlohmann::json& data, const std::string& key);
        static void jsonExistsAndObject(const nlohmann::json& data, const std::string& key);
        static void jsonExistsAndArray(const nlohmann::json& data, const std::string& key);
};

}

#endif //PEGASUS_CONTROLLER_ASSERTION_HPP
