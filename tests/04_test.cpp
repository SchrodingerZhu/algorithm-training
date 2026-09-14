#include <gtest/gtest.h>
import std;
import sieve04;
#include "oracle.hpp"

TEST(Divisors, EnumerationAndOne) {
    Solution solution;
    for (int n : {0, 1, 2, 12, 1000}) {
        const auto actual = solution.divisorStats(n);
        ASSERT_EQ(actual.size(), static_cast<std::size_t>(n + 1));
        for (int x = 0; x <= n; ++x) EXPECT_EQ(actual[x], bruteDivisors(x)) << "x=" << x;
    }
}

TEST(Divisors, PrimePowersAndMixedPowers) {
    Solution solution;
    const auto actual = solution.divisorStats(1'000'000);
    ASSERT_EQ(actual.size(), 1'000'001u);
    for (int x : {64, 72, 210, 729, 83160, 999983, 1000000})
        EXPECT_EQ(actual[x], bruteDivisors(x)) << "x=" << x;
}
