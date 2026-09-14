#include <gtest/gtest.h>
import std;
import tree07;
#include "oracle.hpp"

TEST(Stage07, SmallOracleAndEdgeCases) {
  Solution solution;
  std::mt19937 rng(7107);
  for (int n : {1, 2, 31}) {
    auto p = oracle::parentTree(n, rng);
    std::vector<std::array<long long, 2>> q;
    std::vector<int> expected;
    for (int v = 0; v < n; ++v)
      for (int k = 0; k <= n + 1; ++k) {
        int x = v;
        for (int t = 0; t < k && x != -1; ++t)
          x = p[x];
        q.push_back({v, k});
        expected.push_back(x);
      }
    q.push_back({n - 1, 1000000000000000000LL});
    expected.push_back(-1);
    EXPECT_EQ(solution.kthAncestors(p, q), expected);
  }
}

TEST(Stage07, ScaleAndShape) {
  Solution solution;
  int n = 200000;
  std::vector<int> p(n);
  std::iota(p.begin(), p.end(), -1);
  std::vector<std::array<long long, 2>> q;
  std::vector<int> expected;
  for (int k = 0; k < n; k += 17) {
    q.push_back({n - 1, k});
    expected.push_back(n - 1 - k);
  }
  EXPECT_EQ(solution.kthAncestors(p, q), expected);
}
