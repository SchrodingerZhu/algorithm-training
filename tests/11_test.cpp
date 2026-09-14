#include <gtest/gtest.h>
import std;
import tree11;
#include "oracle.hpp"

TEST(Stage11, SmallOracleAndEdgeCases) {
  Solution solution;
  std::mt19937 rng(7111);
  for (int n : {1, 2, 31}) {
    auto p = oracle::parentTree(n, rng);
    std::vector<long long> w(n);
    for (auto &x : w)
      x = static_cast<int>(rng() % 21) - 10;
    std::vector<std::array<int, 2>> q;
    std::vector<long long> expected;
    for (int u = 0; u < n; ++u)
      for (int v = 0; v < n; ++v) {
        q.push_back({u, v});
        int a = oracle::lca(p, u, v);
        long long value = std::numeric_limits<long long>::max();
        for (int x = u; x != a; x = p[x])
          value = std::min(value, w[x]);
        for (int x = v; x != a; x = p[x])
          value = std::min(value, w[x]);
        expected.push_back(u == v ? 0 : value);
      }
    EXPECT_EQ(solution.pathMin(p, w, q), expected);
  }
}

TEST(Stage11, ScaleAndShape) {
  Solution solution;
  int n = 100000;
  std::vector<int> p(n);
  std::iota(p.begin(), p.end(), -1);
  std::vector<long long> w(n, 9);
  w[0] = -100;
  EXPECT_EQ(solution.pathMin(p, w, {{0, n - 1}, {n - 1, n - 1}}),
            (std::vector<long long>{9, 0}));
}
