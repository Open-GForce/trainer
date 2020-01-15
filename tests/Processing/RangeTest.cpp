#include "../../includes/catch2/catch.hpp"
#include "../../includes/fake_it/single_header/catch/fakeit.hpp"
#include "../../src/Processing/Range.hpp"

using namespace GForce::Processing;

TEST_CASE( "Range tests", "[Processing]" )
{
    SECTION("getLimitedPercentage() => above max => 1 returned")
    {
        auto range = new Range(30, 150);
        CHECK(range->getLimitedPercentage(151) == 1);
    }

    SECTION("getLimitedPercentage() => below min => 0 returned")
    {
        auto range = new Range(30, 150);
        CHECK(range->getLimitedPercentage(29) == 0);
    }

    SECTION("getLimitedPercentage() => corrected percentage")
    {
        auto range = new Range(30, 150);
        CHECK(range->getLimitedPercentage(45) == 0.125);
    }
}