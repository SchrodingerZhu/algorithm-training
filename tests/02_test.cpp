#include <gtest/gtest.h>
import std;
import sieve02;
#include "oracle.hpp"

TEST(FactorCounts, SmallTablesAndOne) {
    Solution solution;
    for (int n : {0, 1, 2, 12, 1000}) {
        const auto actual = solution.factorCounts(n);
        ASSERT_EQ(actual.size(), static_cast<std::size_t>(n + 1));
        for (int x = 0; x <= n; ++x) EXPECT_EQ(actual[x], trialCounts(x)) << "x=" << x;
    }
}

TEST(FactorCounts, PrimePowersAndProducts) {
    Solution solution;
    const auto actual = solution.factorCounts(1'000'000);
    ASSERT_EQ(actual.size(), 1'000'001u);
    for (int x : {64, 210, 729, 2310, 510510, 999983, 1000000})
        EXPECT_EQ(actual[x], trialCounts(x)) << "x=" << x;
}
