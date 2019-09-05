#include "Assertion.hpp"
#include "AssertionFailedException.hpp"

using namespace Pegasus::Networking;

void Assertion::jsonExistsAndNumber(const nlohmann::json &data, const std::string& key)
{
    Assertion::jsonKeyExists(data, key);

    if (!data[key].is_number()) {
        throw AssertionFailedException("Assertion failed. JSON field " + key + " is not a number");
    }
}

void Assertion::jsonExistsAndObject(const nlohmann::json &data, const std::string &key)
{
    Assertion::jsonKeyExists(data, key);

    if (!data[key].is_object()) {
        throw AssertionFailedException("Assertion failed. JSON field " + key + " is not an object");
    }
}

void Assertion::jsonKeyExists(const nlohmann::json &data, const std::string &key)
{
    if (data.find(key) == data.end()) {
        throw AssertionFailedException("Assertion failed. Missing JSON field " + key);
    }
}