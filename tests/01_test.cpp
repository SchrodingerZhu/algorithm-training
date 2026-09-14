#include <gtest/gtest.h>
import std;
import sieve01;
#include "oracle.hpp"

TEST(Euler, BoundariesAndSmallTables) {
    Solution solution;
    for (int n : {0, 1, 2, 3, 12, 50, 1000}) {
        const auto lp = solution.leastPrimeFactors(n);
        ASSERT_EQ(lp.size(), static_cast<std::size_t>(n + 1));
        for (int x = 0; x <= n; ++x) EXPECT_EQ(lp[x], trialLeastPrime(x)) << "x=" << x;
    }
}

TEST(Euler, MillionAndRepeatCalls) {
    Solution solution;
    const auto lp = solution.leastPrimeFactors(1'000'000);
    ASSERT_EQ(lp.size(), 1'000'001u);
    int primeCount = 0;
    for (int x = 2; x <= 1'000'000; ++x) primeCount += lp[x] == x;
    EXPECT_EQ(primeCount, 78498);
    for (int x = 2; x <= 1'000'000; x += 1999) EXPECT_EQ(lp[x], trialLeastPrime(x));
    EXPECT_EQ(solution.leastPrimeFactors(2), (std::vector<int>{0, 0, 2}));
}
