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
    UNIFORMLY_SPACED,
    EXPONENTIALLY_SPACED,
    MID_POINT,
    UNIFORMLY_RANDOM,
    EXPONENTIALLY_RANDOM
};
/****************************************************************************/
// This class serves as a base and as a factory for specific implementations
// of distributions
// Example of use:
//   //Create a uniformly spaced generator for 20 points between 10 and 100
//   DistributionGenerator *generator = DistributionGenerator::make_generator(10, 100, UNIFORMLY_SPACED, 20);
//   //Print all points
//   while ( ! generator->is_done() ) std::cout << generator->next() << std::endl;
class DistributionGenerator {
    protected:
        uint_fast64_t min_;          //Lower limit (included)
        uint_fast64_t max_;          //Upper limit (not included)
        uint_fast64_t count_limit_;  //Number of points that can be generated
        uint_fast64_t count_ = 0;    //Number of points already generated

    public:
        //Factory method
        static DistributionGenerator *make_generator (
            uint_fast64_t min, uint_fast64_t max, Generators generator_kind,
            uint_fast64_t count_limit );
        //Returns true if the generation limit has been achieved
        bool is_done (  ) const { return ! (count_ < count_limit_); }
        //Returns a point the in the distribution and increments the counter
        virtual uint_fast64_t next (  ) = 0;
};

// Distribution Generator that only provides the average point between limits
// Example: AveragePoint(0,10) gives 5
// [0] - 1 - 2 - 3 - 4 - (5) - 6 - 7 - 8 - 9 - [10]
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

// Distribution Generator that splits the interval in count_limit_ + 1 parts
// Example: UniformlySpaced(0,10,1) gives 5
// [0] - 1 - 2 - 3 - 4 - (5) - 6 - 7 - 8 - 9 - [10]
// Example: UniformlySpaced(0,10,4) gives 2, 4, 6, and 8
// [0] - 1 - (2) - 3 - (4) - 5 - (6) - 7 - (8) - 9 - [10]
class UniformlySpaced : public DistributionGenerator {
    public:
        //Constructor with lower and upper limit and the number of parts to
        //break the space
        //Limits are taken care by the factory
        UniformlySpaced (uint_fast64_t min, uint_fast64_t max,
                uint_fast64_t count_limit) {
            min_ = min;
            max_ = max;
            count_limit_ = count_limit + 1ul;
            count_ = 1ul;
        }
        //Provides the point splitting the next intervals
        uint_fast64_t next (  );
};

// Distribution Generator that splits the interval in count_limit + 1 parts.
// Each part is exponentially larger than the one before
// Example: ExponentiallySpaced(2,8,1) gives 4
// [2] - 3 - (4) - 5 - 6 - 7 - [8]
// Example: ExponentiallySpaced(2,16,2) gives 4 and 8
// [2] - 3 - (4) - 5 - 6 - 7 - (8) - 9 - 10 - 11 - 12 - 13 - 14 - 15 - [16]
class ExponentiallySpaced : public UniformlySpaced {
    public:
        //Constructor with lower and upper limit and the number of parts to
        //break the space
        //Limits are taken care by the factory
        //We only guarantee that we will not do log operations with zero
        ExponentiallySpaced (uint_fast64_t min, uint_fast64_t max,
                uint_fast64_t count_limit) :
            UniformlySpaced ( (min != 0ul ? min : 1ul), max, count_limit ){  }
        //Provides the point splitting the next intervals in an exponential scale
        uint_fast64_t next (  );
};

// Distribution Generator that only provides the mid point between limits
// A mid point is the average point in an exponential scale between two limits
// It behaves similar to an ExponentiallySpaced interval divided into two parts
// Example: MidPoint(2,8) gives 4
// [2] - 3 - (4) - 5 - 6 - 7 - [8]
class MidPoint : public AveragePoint {
    public:
        //Usual constructor with lower and upper limit
        //Limits are taken care by the factory
        MidPoint (uint_fast64_t min, uint_fast64_t max) :
            AveragePoint ( (min != 0ul ? min : 1ul), max ){  }
        //Provides the mid point between lower and upper limits
        uint_fast64_t next (  );
};

// Distribution Generator that generates count_limit_ random points.
// Example: UniformlyRandom(2,8,1) gives anything between 2 and 8 included
class UniformlyRandom : public DistributionGenerator {
    private:
        //Uniform distribution to generate random numbers
        std::uniform_int_distribution<uint_fast64_t> randomize;
        //Generator of random numbers
        std::random_device device;
    public:
        //Constructor with lower and upper limit and the number of random
        //points to generate
        //Limits are taken care by the factory
        UniformlyRandom (uint_fast64_t min, uint_fast64_t max,
                uint_fast64_t count_limit) {
            min_ = min;
            max_ = max;
            count_limit_ = count_limit;
            randomize = std::uniform_int_distribution<uint_fast64_t>(min, max);
        }
        //Provides the point splitting the next intervals in an exponential scale
        uint_fast64_t next (  );
};

// Distribution Generator that generates count_limit_ random points
// It follows the same spacing idea of ExponentiallySpaced
// Example: ExponentiallyRandom(2,8,4) gives 4 numbers between 2 and 8 included
class ExponentiallyRandom : public DistributionGenerator {
    private:
        //Uniform distribution to generate random numbers
        std::uniform_real_distribution<double> randomize;
        //Generator of random numbers
        std::random_device device;
        //Min and Max in log scale
        double log_min_, log_max_;
    public:
        //Constructor with lower and upper limit and the number of random
        //points to generate
        //Limits are taken care by the factory
        ExponentiallyRandom (uint_fast64_t min, uint_fast64_t max,
                uint_fast64_t count_limit) {
            min_ = min;
            max_ = max;
            log_min_ = log (min);
            log_max_ = log (max);
            count_limit_ = count_limit;
            randomize = std::uniform_real_distribution<double>(log_min_, log_max_);
        }
        //Provides the point splitting the next intervals in an exponential scale
        uint_fast64_t next (  );

};
/****************************************************************************/
// Method implementations

// Second implementation: can create an Average Point or a Uniformly Spaced
// distribution.
DistributionGenerator *DistributionGenerator::make_generator (
        uint_fast64_t min, uint_fast64_t max,
        Generators generator_kind = AVERAGE_POINT,
        uint_fast64_t count_limit=1lu ) {
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
    else if ( generator_kind == UNIFORMLY_SPACED )
        return new UniformlySpaced(min, max, count_limit);
    else if ( generator_kind == EXPONENTIALLY_SPACED )
        return new ExponentiallySpaced(min, max, count_limit);
    else if ( generator_kind == MID_POINT )
        return new MidPoint(min, max);
    else if ( generator_kind == UNIFORMLY_RANDOM )
        return new UniformlyRandom(min, max, count_limit);
    else
        return new ExponentiallyRandom(min, max, count_limit);
}


// AveragePoint generation operation
// Returns the average point and counts the call
// Since we are using 64b numbers, it is not checking for overflows
uint_fast64_t AveragePoint::next (  ) {
    ++count_;
    return ( max_ + min_ ) >> 1;
}

// UniformlySpaced generation operation
// Breaks the space in count_limit_ (+ 1) pieces, returns the n-th (count) point,
// and counts the call
uint_fast64_t UniformlySpaced::next (  ) {
    uint_fast64_t point = min_ + ( count_ * ( max_ - min_ )) / count_limit_;
    ++count_;
    return point;
}

// ExponentiallySpaced generation operation
// Breaks the space in count_limit_ (+ 1) pieces, returns the n-th (count) point in
// an exponential scale
// and counts the call
uint_fast64_t ExponentiallySpaced::next (  ) {
    double log_min = log ( min_ );
    double log_max = log ( max_ );
    double point = exp ( log_min + (count_ * ( log_max - log_min ) ) / count_limit_ );
    ++count_;
    return (uint_fast64_t) round ( point ); // Handling any rounding errors
}

// MidPoint generation operation
// Divides the interval into two parts in an exponential scale
// and counts the call
uint_fast64_t MidPoint::next (  ) {
    double log_min = log ( min_ );
    double log_max = log ( max_ );
    double point = exp ( log_min + ( log_max - log_min ) / 2 );
    ++count_;
    return (uint_fast64_t) round ( point ); // Handling any rounding errors
}

// UniformlyRandom generation operation
// Gets a number in a uniform_int_distribution
uint_fast64_t UniformlyRandom::next (  ) {
    ++count_;
    return randomize ( device );
}

// ExponentiallyRandom generation operation
// Gets a number in a uniform_real_distribution and converts it to a point in
// the original limits
uint_fast64_t ExponentiallyRandom::next (  ) {
    ++count_;
    return exp ( randomize ( device ) );
}

