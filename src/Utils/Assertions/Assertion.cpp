#include "Assertion.hpp"
#include "AssertionFailedException.hpp"

using namespace GForce::Utils::Assertions;

void Assertion::jsonExistsAndNumber(const nlohmann::json &data, const std::string& key)
{
    Assertion::jsonKeyExists(data, key);

    if (!data[key].is_number()) {
        throw AssertionFailedException("Assertion failed. JSON field " + key + " is not a number");
    }
}

void Assertion::jsonExistsAndString(const nlohmann::json &data, const std::string &key)
{
    Assertion::jsonKeyExists(data, key);

    if (!data[key].is_string()) {
        throw AssertionFailedException("Assertion failed. JSON field " + key + " is not a string");
    }
}

void Assertion::jsonExistsAndBool(const nlohmann::json &data, const std::string &key)
{
    Assertion::jsonKeyExists(data, key);

    if (!data[key].is_boolean()) {
        throw AssertionFailedException("Assertion failed. JSON field " + key + " is not boolean");
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