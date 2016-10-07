#pragma once

#include <random>
#include <algorithm>

using namespace std;

// Classes in this file
class DistributionGenerator;
class AveragePoint;
class UniformlySpaced;

enum Generators {
    AVERAGE_POINT,
    UNIFORMLY_SPACED
};

// Example:
// The class serves as a base and as a factory for specific implementations
// of distributions
class DistributionGenerator {
    protected:
        uint_fast64_t min_;          //Lower limit (included)
        uint_fast64_t max_;          //Upper limit (not included)
        uint_fast64_t count_limit_;  //Number of points that can be generated
        uint_fast64_t count_ = 0;    //Number of points already generated

    public:
        //Factory method
        static DistributionGenerator *make_generator (
            uint_fast64_t min, uint_fast64_t max, uint_fast64_t count_limit,
            Generators generator_kind );
        //Returns true if the generation limit has been achieved
        bool is_done (  ) const { return ! (count_ < count_limit_); }
        //Returns a point the in the distribution and increments the count
        virtual uint_fast64_t next (  ) = 0;
};

// Distribution Generator that only provides the average point between limits
class AveragePoint : public DistributionGenerator {
    public:
        //Usual constructor with lower and upper limit
        //Limits are taken care by the factory
        AveragePoint (uint_fast64_t min, uint_fast64_t max) {
            min_ = min;
            max_ = max;
            count_limit_ = 1;
        }
        //Provides the average point between lower and upper limits
        uint_fast64_t next (  );
};

class UniformlySpaced : public DistributionGenerator {
    public:
        //Constructor with lower and upper limit and the number of parts to
        //break the space
        //Limits are taken care by the factory
        UniformlySpaced (uint_fast64_t min, uint_fast64_t max,
                uint_fast64_t count_limit) {
            min_ = min;
            max_ = max;
            count_limit_ = count_limit;
        }
        //Provides the average point between lower and upper limits
        uint_fast64_t next (  );
};


// Method implementations

// Second implementation: can create an Average Point or a Uniformly Spaced
// distribution.
DistributionGenerator *DistributionGenerator::make_generator (
        uint_fast64_t min, uint_fast64_t max, uint_fast64_t count_limit=1lu,
        Generators generator_kind = AVERAGE_POINT ) {
    // First test: min < max -> swaps values to fix it
    if ( min > max ) swap(min, max);
    // Second test: min == max -> increases or decreases one of them
    if ( min == max ) {
        if ( max < UINT_FAST64_MAX ) ++max;
        else --min; // decreases min since we cannot increase max
    }
    // Third test: count_limit == 0 -> swap its value to at least 1
    if ( count_limit == 0 ) count_limit = 1lu;

    // Creates
    if ( generator_kind == AVERAGE_POINT )
        return new AveragePoint(min, max);
    else
        return new UniformlySpaced(min, max, count_limit);
}

// AveragePoint generation operation
// Returns the average point and counts the call
uint_fast64_t AveragePoint::next (  ) {
    ++count_;
    return ( max_ + min_ ) / 2;
}

// UniformlySpaced generation operation
// Breaks the space in count_limit pieces, returns the n-th (count) point,
// and counts the calls
uint_fast64_t UniformlySpaced::next (  ) {
    uint_fast64_t part = min_ + (count_ * ( max_ - min_ )) / count_limit_;
    ++count_;
    return part;
}
