#include <gtest/gtest.h>
import std;
import sieve06;
#include "oracle.hpp"

TEST(CoprimePairs, ExhaustiveSmallRectangles) {
    Solution solution;
    for (int n = 0; n <= 24; ++n)
        for (int m = 0; m <= 24; ++m)
            EXPECT_EQ(solution.countCoprimePairs(n, m), bruteCoprimePairs(n, m)) << n << ',' << m;
}

TEST(CoprimePairs, LargeRectanglesAnd64BitResult) {
    Solution solution;
    constexpr int n = 1'000'000;
    std::vector<int> phi(n + 1);
    std::iota(phi.begin(), phi.end(), 0);
    for (int p = 2; p <= n; ++p) if (phi[p] == p)
        for (int x = p; x <= n; x += p) phi[x] -= phi[x] / p;
    const auto expected = 2LL * std::accumulate(phi.begin() + 1, phi.end(), 0LL) - 1;
    EXPECT_GT(expected, std::numeric_limits<int>::max());
    EXPECT_EQ(solution.countCoprimePairs(n, n), expected);
    EXPECT_EQ(solution.countCoprimePairs(n, 2), 1'500'000);
    EXPECT_EQ(solution.countCoprimePairs(2, n), 1'500'000);
    EXPECT_EQ(solution.countCoprimePairs(0, n), 0);
    EXPECT_EQ(solution.countCoprimePairs(n, 1), n);
}
