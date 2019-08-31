#include "Range.hpp"

using namespace GForce::Processing;

Range::Range(int min, int max) : min(min), max(max) {}

Range *Range::clone() {
    return new Range(min, max);
}

double Range::getLimitedPercentage(int value)
{
    if (value >= this->max) {
        return 1;
    }

    if (value <= this->min) {
        return 0;
    }

    int delta = this->max - this->min;
    value -= this->min;

    return (double) value / delta;
}

int Range::getMin() const {
    return min;
}

int Range::getMax() const {
    return max;
}

