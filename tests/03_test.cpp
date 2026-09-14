#include <gtest/gtest.h>
import std;
import tree03;
#include "oracle.hpp"

TEST(Stage03, SmallOracleAndEdgeCases) {
  Solution solution;
  std::mt19937 rng(7103);
  for (int n : {1, 2, 9, 31}) {
    std::vector<long long> a(n);
    for (auto &x : a)
      x = static_cast<int>(rng() % 11) - 5;
    for (int k = 1; k <= n; ++k) {
      std::vector<long long> expected;
      for (int l = 0; l + k <= n; ++l)
        expected.push_back(oracle::minimum(a, l, l + k - 1));
      EXPECT_EQ(solution.windowMin(a, k), expected);
    }
  }
}

TEST(Stage03, ScaleAndShape) {
  Solution solution;
  std::vector<long long> a(200000, 7);
  std::vector<long long> expected(100001, 7);
  EXPECT_EQ(solution.windowMin(a, 100000), expected);
  std::iota(a.begin(), a.end(), 0LL);
  EXPECT_EQ(solution.windowMin(a, 200000), (std::vector<long long>{0}));
}
