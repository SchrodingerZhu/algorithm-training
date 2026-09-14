#include <gtest/gtest.h>
import std;
import tree04;
#include "oracle.hpp"

TEST(Stage04, SmallOracleAndEdgeCases) {
  Solution solution;
  std::mt19937 rng(7104);
  std::vector<long long> initial{5, -2, -2, 8, 0}, a = initial;
  std::vector<long long> expected;
  std::vector<std::array<long long, 3>> ops;
  for (int i = 0; i < 300; ++i) {
    int l = rng() % 5, r = rng() % 5;
    if (l > r)
      std::swap(l, r);
    long long v = static_cast<int>(rng() % 31) - 15;
    if (i % 2 == 0) {
      ops.push_back({0, l, v});
      a[l] = v;
    } else {
      ops.push_back({1, l, r});
      expected.push_back(oracle::minimum(a, l, r));
    }
  }
  EXPECT_EQ(solution.rangeMinUpdates(initial, ops), expected);
  EXPECT_TRUE(solution.rangeMinUpdates({1}, {}).empty());
}

TEST(Stage04, ScaleAndShape) {
  Solution solution;
  const int n = 50000;
  std::vector<long long> a(n, 9), expected;
  std::vector<std::array<long long, 3>> ops;
  for (int i = 0; i < 20000; ++i) {
    ops.push_back({0, 0, -1LL * i});
    ops.push_back({1, 0, n - 1});
    expected.push_back(-i);
  }
  EXPECT_EQ(solution.rangeMinUpdates(a, ops), expected);
}
