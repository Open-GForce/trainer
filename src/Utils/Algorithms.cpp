#include <sstream>
#include "Algorithms.hpp"

using namespace GForce::Utils;

std::string Algorithms::implodeVector(const std::vector<int>& data)
{
    std::stringstream stream;
    for(size_t i = 0; i < data.size(); ++i){
        if(i != 0)
            stream << ",";
        stream << data[i];
    }

    return stream.str();
}
