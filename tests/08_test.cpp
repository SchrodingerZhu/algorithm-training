#include <gtest/gtest.h>
import std;
import tree08;
#include "oracle.hpp"

TEST(Stage08, SmallOracleAndEdgeCases) {
  Solution solution;
  std::mt19937 rng(7108);
  for (int n : {1, 2, 31}) {
    auto p = oracle::parentTree(n, rng);
    std::vector<std::array<int, 2>> q;
    std::vector<int> expected;
    for (int u = 0; u < n; ++u)
      for (int v = 0; v < n; ++v) {
        q.push_back({u, v});
        expected.push_back(oracle::lca(p, u, v));
      }
    EXPECT_EQ(solution.lca(p, q), expected);
  }
}

TEST(Stage08, ScaleAndShape) {
  Solution solution;
  int n = 100000;
  std::vector<int> p(n);
  std::iota(p.begin(), p.end(), -1);
  std::vector<std::array<int, 2>> q;
  std::vector<int> expected;
  for (int v = 0; v < n; v += 13) {
    q.push_back({n - 1, v});
    expected.push_back(v);
  }
  EXPECT_EQ(solution.lca(p, q), expected);
  std::fill(p.begin() + 1, p.end(), 0);
  std::fill(expected.begin(), expected.end(), 0);
  EXPECT_EQ(solution.lca(p, q), expected);
}
