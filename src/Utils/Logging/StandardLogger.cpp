#include <iostream>
#include "StandardLogger.hpp"

using namespace GForce::Utils::Logging;

void StandardLogger::error(std::string message)
{
    std::cerr << "Error: " << message << "\n";
}

void StandardLogger::info(std::string message)
{
    std::cout << "Info: " << message << "\n";
}
