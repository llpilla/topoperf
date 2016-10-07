#include "simple_tester.hpp"

#include "../src/distribution_generator.hpp"

void test_AveragePoint (  ) {
    DistributionGenerator *generator;

    DESCRIBE("Average Point Distribution");
    WHEN("I create an Average Point from 0 to 10");
    IFTHEN("I generate a point", "the results should be 5");
    generator = DistributionGenerator::make_generator(0,10);
    isEqual(generator->next(), (uint_fast64_t) 5);

    IFTHEN("I generate another point", "it should be 5 again");
    isEqual(generator->next(), (uint_fast64_t) 5);

    IFTHEN("I check if it is done", "it should be done");
    isTrue(generator->is_done());

    WHEN("I create another Average Point");
    IFTHEN("I check if it is done", "it should not be done");
    generator = DistributionGenerator::make_generator(0,10);
    isFalse(generator->is_done());

    WHEN("I create an Average Point from 1000 to 0");
    IFTHEN("I generate a point", "the results should be 500");
    generator = DistributionGenerator::make_generator(1000,0);
    isEqual(generator->next(), (uint_fast64_t) 500);

    WHEN("I create an Average Point with two equal values");
    IFTHEN("I generate a point", "the results should be equal to the original value due to rounding");
    generator = DistributionGenerator::make_generator(256,256);
    isEqual(generator->next(), (uint_fast64_t) 256);

}

void test_UniformlySpaced (  ) {
    DistributionGenerator *generator;

    DESCRIBE("Uniformly Spaced");
    WHEN("I create a Uniform Distribution from 0 to 100 with 10 points");
    IFTHEN("I generate a point", "the results should be 0");
    generator = DistributionGenerator::make_generator(0, 100, 10, UNIFORMLY_SPACED);
    isEqual(generator->next(), (uint_fast64_t) 0);
    IFTHEN("I generate a second point", "the results should be 10");
    isEqual(generator->next(), (uint_fast64_t) 10);

    WHEN("I create another Uniform Distribution with 5 points");
    IFTHEN("I generate all points", "it should stop after 5 requests");
    generator = DistributionGenerator::make_generator(0, 100, 5, UNIFORMLY_SPACED);
    int i = 0;
    uint_fast64_t result;
    while ( ! generator->is_done() ) {
        result = generator->next();
        ++i;
    }
    isEqual(i, 5);
    IFTHEN("I generated all points", "the last one should be equal to 80");
    isEqual(result, (uint_fast64_t) 80);

}

int main () {
    test_AveragePoint();
    test_UniformlySpaced();
}
