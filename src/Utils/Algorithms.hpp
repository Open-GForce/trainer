#ifndef GFORCE_TRAINER_CONTROLLER_ALGORITHMS_HPP
#define GFORCE_TRAINER_CONTROLLER_ALGORITHMS_HPP

#include <string>
#include <vector>

namespace GForce::Utils {

class Algorithms
{
    public:
        /**
         * Converts the vector content to comma seperated strings
         */
        static std::string implodeVector(const std::vector<int>& data);
};

}

#endif //GFORCE_TRAINER_CONTROLLER_ALGORITHMS_HPP
