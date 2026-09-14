#include <gtest/gtest.h>
import std;
import tree16;
#include "oracle.hpp"

TEST(Stage16, SmallOracleAndEdgeCases) {
  Solution solution;
  std::mt19937 rng(7116);
  std::vector<long long> initial{1, -2, 3, 4, 5, -6, 7}, a = initial, expected;
  std::vector<std::array<int, 3>> ops;
  for (int i = 0; i < 500; ++i) {
    int l = rng() % a.size(), r = rng() % a.size();
    if (l > r)
      std::swap(l, r);
    int type = i % 3 == 0 ? 1 : 0;
    ops.push_back({type, l, r});
    if (type == 0)
      std::reverse(a.begin() + l, a.begin() + r + 1);
    else
      expected.push_back(
          std::accumulate(a.begin() + l, a.begin() + r + 1, 0LL));
  }
  EXPECT_EQ(solution.reverseSum(initial, ops), expected);
  EXPECT_EQ(solution.reverseSum({-5}, {{0, 0, 0}, {1, 0, 0}}),
            (std::vector<long long>{-5}));
}

TEST(Stage16, ScaleAndShape) {
  Solution solution;
  int n = 100000;
  std::vector<long long> a(n);
  std::iota(a.begin(), a.end(), 1LL);
  std::vector<std::array<int, 3>> ops;
  std::vector<long long> expected;
  for (int i = 0; i < 20000; ++i) {
    ops.push_back({0, 0, n - 1});
    ops.push_back({1, 0, 0});
    expected.push_back(i % 2 == 0 ? n : 1);
  }
  EXPECT_EQ(solution.reverseSum(a, ops), expected);
}
