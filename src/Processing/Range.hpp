#ifndef GFORCE_TRAINER_CONTROLLER_RANGE_HPP
#define GFORCE_TRAINER_CONTROLLER_RANGE_HPP

namespace GForce::Processing {

class Range
{
    private:
        int min;
        int max;

    public:
        Range(int min, int max);
        Range* clone();

        double getLimitedPercentage(int value);

        int getMin() const;
        int getMax() const;
};

}

#endif //GFORCE_TRAINER_CONTROLLER_RANGE_HPP
