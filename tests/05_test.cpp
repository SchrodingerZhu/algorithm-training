#include <gtest/gtest.h>
import std;
import sieve05;
#include "oracle.hpp"

TEST(Mobius, SmallTables) {
    Solution solution;
    for (int n : {0, 1, 2, 12, 1000}) EXPECT_EQ(solution.mobius(n), mobiusByDivisorIdentity(n));
}

TEST(Mobius, MillionComparedWithIndependentDivisorSums) {
    Solution solution;
    EXPECT_EQ(solution.mobius(1'000'000), mobiusByDivisorIdentity(1'000'000));
}
