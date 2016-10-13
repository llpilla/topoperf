#include "simple_tester.hpp"

#include "../src/distribution_generator.hpp"

void test_AveragePoint (  ) {
    DistributionGenerator *generator;

    DESCRIBE("Average Point Distribution");

    WHEN("I create an Average Point from 2 to 12");
    IFTHEN("I generate a point", "the result should be 7");
    generator = DistributionGenerator::make_generator(2, 12);
    isEqual(generator->next(), (uint_fast64_t) 7);

    IFTHEN("I generate another point", "it should be 7 again");
    isEqual(generator->next(), (uint_fast64_t) 7);

    IFTHEN("I check if it is done", "it should be done");
    isTrue(generator->is_done());

    WHEN("I create another Average Point");
    IFTHEN("I check if it is done", "it should not be done");
    generator = DistributionGenerator::make_generator(0, 10);
    isFalse(generator->is_done());

    WHEN("I create an Average Point from 1000 to 0");
    IFTHEN("I generate a point", "the result should be 500");
    generator = DistributionGenerator::make_generator(1000, 0);
    isEqual(generator->next(), (uint_fast64_t) 500);

    WHEN("I create an Average Point with two equal values");
    IFTHEN("I generate a point", "the result should be equal to the original value due to rounding");
    generator = DistributionGenerator::make_generator(256, 256);
    isEqual(generator->next(), (uint_fast64_t) 256);

}

void test_UniformlySpaced (  ) {
    DistributionGenerator *generator;

    DESCRIBE("Uniformly Spaced");

    WHEN("I create an Uniform Distribution from 2 to 12 with 1 point (2 intervals)");
    IFTHEN("I generate a point", "the result should be 7");
    generator = DistributionGenerator::make_generator(2, 12, UNIFORMLY_SPACED, 1);
    isEqual(generator->next(), (uint_fast64_t) 7);

    WHEN("I create a Uniform Distribution from 0 to 100 with 9 points (10 intervals)");
    IFTHEN("I generate a point", "the result should be 10");
    generator = DistributionGenerator::make_generator(0, 100, UNIFORMLY_SPACED, 9);
    isEqual(generator->next(), (uint_fast64_t) 10);
    IFTHEN("I generate a second point", "the result should be 20");
    isEqual(generator->next(), (uint_fast64_t) 20);

    WHEN("I create another Uniform Distribution with 4 points (5 intervals)");
    IFTHEN("I generate all points", "it should stop after 4 requests");
    generator = DistributionGenerator::make_generator(0, 100, UNIFORMLY_SPACED, 4);
    int i = 0;
    uint_fast64_t result;
    while ( ! generator->is_done() ) {
        result = generator->next();
        ++i;
    }
    isEqual(i, 4);
    IFTHEN("I generated all points", "the last one should be equal to 80");
    isEqual(result, (uint_fast64_t) 80);

    WHEN("I create a Uniform Distribution from 1 to 2^20 with 10000 points");
    IFTHEN("I generate points", "they should never be smaller than their previous point");
    generator = DistributionGenerator::make_generator(1, 1024*1024, UNIFORMLY_SPACED, 10000);
    bool not_smaller = true;
    uint_fast64_t previous = 0ul;
    uint_fast64_t current;
    while ( (! generator->is_done()) && not_smaller ) {
        current = generator->next();
        not_smaller = previous <= current;
        previous = current;
    }
    isTrue(not_smaller);
}

void test_ExponentiallySpaced (  ) {
    DistributionGenerator *generator;

    DESCRIBE("Exponentially Spaced");

    WHEN("I create an Exponential Distribution from 2 to 2048 with 9 points (10 intervals)");
    IFTHEN("I generate a point", "the result should be 4");
    generator = DistributionGenerator::make_generator(2, 2048, EXPONENTIALLY_SPACED, 9);
    isEqual(generator->next(), (uint_fast64_t) 4);
    IFTHEN("I generate the next point", "the result should be 8");
    isEqual(generator->next(), (uint_fast64_t) 8);
    IFTHEN("I generate the next point", "the result should be 16");
    isEqual(generator->next(), (uint_fast64_t) 16);

    WHEN("I create another Exponential Distribution with 4 points (5 intervals)");
    IFTHEN("I generate all points", "it should stop after 4 requests");
    generator = DistributionGenerator::make_generator(0, 100, EXPONENTIALLY_SPACED, 4);
    int i = 0;
    uint_fast64_t result;
    while ( ! generator->is_done() ) {
        result = generator->next();
        ++i;
    }
    isEqual(i, 4);

    WHEN("I create an Exponential Distribution from 1 to 2^20 with 10000 points");
    IFTHEN("I generate points", "they should never be smaller than their previous point");
    generator = DistributionGenerator::make_generator(1, 1024*1024, EXPONENTIALLY_SPACED, 10000);
    bool not_smaller = true;
    uint_fast64_t previous = 0ul;
    uint_fast64_t current;
    while ( (! generator->is_done()) && not_smaller ) {
        current = generator->next();
        not_smaller = previous <= current;
        previous = current;
    }
    isTrue(not_smaller);
}

void test_MidPoint (  ) {
    DistributionGenerator *generator;

    DESCRIBE("Mid Point Distribution");

    WHEN("I create a Mid Point from 2 to 32");
    IFTHEN("I generate a point", "the result should be 8");
    generator = DistributionGenerator::make_generator(2, 32, MID_POINT);
    isEqual(generator->next(), (uint_fast64_t) 8);

    IFTHEN("I generate another point", "it should be 8 again");
    isEqual(generator->next(), (uint_fast64_t) 8);

    IFTHEN("I check if it is done", "it should be done");
    isTrue(generator->is_done());

    WHEN("I create another Mid Point");
    IFTHEN("I check if it is done", "it should not be done");
    generator = DistributionGenerator::make_generator(1, 10, MID_POINT);
    isFalse(generator->is_done());

}


void test_UniformlyRandom (  ) {
    DistributionGenerator *generator;

    DESCRIBE("Uniformly Random");

    WHEN("I create a Random Uniform Distribution from 10 to 1000 with 100 points");
    IFTHEN("I generate all points", "they should all be between 10 and 1000");
    generator = DistributionGenerator::make_generator(10, 1000, UNIFORMLY_RANDOM, 100);
    bool inside_interval = true;
    uint_fast64_t point = 0ul;
    int i = 0;
    while ( (! generator->is_done()) && inside_interval ) {
        point = generator->next();
        ++i;
        inside_interval = ( point >= 10ul ) && ( point <= 1000ul );
    }
    isTrue(inside_interval);
    IFTHEN("I generate all points", "it should stop after 100 requests");
    isEqual(i, 100);
}


int main () {
    test_AveragePoint();
    test_UniformlySpaced();
    test_ExponentiallySpaced();
    test_MidPoint();
    test_UniformlyRandom();
}
