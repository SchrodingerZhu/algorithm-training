#include <gtest/gtest.h>
import std;
import sieve03;
#include "oracle.hpp"

TEST(Totients, Enumeration) {
    Solution solution;
    for (int n : {0, 1, 2, 36, 200}) {
        const auto actual = solution.totients(n);
        ASSERT_EQ(actual.size(), static_cast<std::size_t>(n + 1));
        for (int x = 0; x <= n; ++x) EXPECT_EQ(actual[x], brutePhi(x)) << "x=" << x;
    }
}

TEST(Totients, MillionAndDivisorIdentity) {
    Solution solution;
    const auto actual = solution.totients(1'000'000);
    ASSERT_EQ(actual.size(), 1'000'001u);
    for (int x : {64, 210, 729, 2310, 510510, 999983, 1000000}) {
        long long sum = 0;
        for (int d = 1; d <= x; ++d) if (x % d == 0) sum += actual[d];
        EXPECT_EQ(sum, x) << "x=" << x;
        EXPECT_EQ(actual[x], brutePhi(x)) << "x=" << x;
    }
}
