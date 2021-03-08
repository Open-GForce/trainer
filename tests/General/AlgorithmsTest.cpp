#include <catch2/catch.hpp>
#include "../../src/Utils/Algorithms.hpp"

using namespace GForce::Utils;

TEST_CASE( "Implode vector", "[Algorithms]" ) {
    SECTION("Filled int vector")
    {
        std::vector<int> data = {4976, -1734, 512};
        CHECK(Algorithms::implodeVector(data) == "4976,-1734,512");
    }

    SECTION("Empty int vector")
    {
        std::vector<int> data = {};
        CHECK(Algorithms::implodeVector(data).empty());
    }
}