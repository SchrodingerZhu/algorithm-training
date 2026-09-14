#include <gtest/gtest.h>
import std;
import tree05;
#include "oracle.hpp"

TEST(Stage05, SmallOracleAndEdgeCases) {
  Solution solution;
  std::mt19937 rng(7104);
  std::vector<long long> initial{5, -2, -2, 8, 0}, a = initial;
  std::vector<long long> expected;
  std::vector<std::array<long long, 4>> ops;
  for (int i = 0; i < 300; ++i) {
    int l = rng() % 5, r = rng() % 5;
    if (l > r)
      std::swap(l, r);
    long long v = static_cast<int>(rng() % 31) - 15;
    if (i % 2 == 0) {
      ops.push_back({0, l, r, v});
      for (int j = l; j <= r; ++j)
        a[j] += v;
    } else {
      ops.push_back({1, l, r, 0});
      expected.push_back(oracle::minimum(a, l, r));
    }
  }
  EXPECT_EQ(solution.rangeAddMin(initial, ops), expected);
  EXPECT_TRUE(solution.rangeAddMin({1}, {}).empty());
}

TEST(Stage05, ScaleAndShape) {
  Solution solution;
  const int n = 50000;
  std::vector<long long> a(n, 9), expected;
  std::vector<std::array<long long, 4>> ops;
  for (int i = 0; i < 20000; ++i) {
    ops.push_back({0, 0, n - 1, -1000000});
    ops.push_back({1, 0, n - 1, 0});
    expected.push_back(9 - 1000000LL * (i + 1));
  }
  EXPECT_EQ(solution.rangeAddMin(a, ops), expected);
}
