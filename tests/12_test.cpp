#include <gtest/gtest.h>
import std;
import tree12;
#include "oracle.hpp"

TEST(Stage12, SmallOracleAndEdgeCases) {
  Solution solution;
  std::mt19937 rng(7112);
  auto p = oracle::parentTree(19, rng);
  std::vector<std::array<int, 3>> q;
  std::vector<int> expected;
  for (int r = 0; r < 19; ++r) {
    auto pr = oracle::reroot(p, r);
    for (int u = 0; u < 19; ++u)
      for (int v = 0; v < 19; ++v) {
        q.push_back({r, u, v});
        expected.push_back(oracle::lca(pr, u, v));
      }
  }
  EXPECT_EQ(solution.rerootedLca(p, q), expected);
}

TEST(Stage12, ScaleAndShape) {
  Solution solution;
  int n = 100000;
  std::vector<int> p(n);
  std::iota(p.begin(), p.end(), -1);
  std::vector<std::array<int, 3>> q;
  std::vector<int> expected;
  for (int i = 0; i < 20000; ++i) {
    int r = (i * 7919LL) % n;
    q.push_back({r, 0, n - 1});
    expected.push_back(r);
  }
  EXPECT_EQ(solution.rerootedLca(p, q), expected);
}
