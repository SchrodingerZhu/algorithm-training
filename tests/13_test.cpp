#include <gtest/gtest.h>
import std;
import tree13;
#include "oracle.hpp"

TEST(Stage13, SmallOracleAndEdgeCases) {
  Solution solution;
  std::mt19937 rng(7113);
  auto p = oracle::parentTree(23, rng);
  std::vector<long long> initial(23), a;
  for (auto &x : initial)
    x = static_cast<int>(rng() % 21) - 10;
  a = initial;
  std::vector<std::array<long long, 3>> ops;
  std::vector<long long> expected;
  for (int i = 0; i < 400; ++i) {
    int u = rng() % 23;
    long long value = static_cast<int>(rng() % 31) - 15;
    if (i % 2 == 0) {
      ops.push_back({0, u, value});
      a[u] = value;
    } else {
      long long best = std::numeric_limits<long long>::max();
      ops.push_back({1, u, 0});
      for (int x = 0; x < 23; ++x)
        if (oracle::lca(p, u, x) == u)
          best = std::min(best, a[x]);
      expected.push_back(best);
    }
  }
  EXPECT_EQ(solution.subtreeMin(p, initial, ops), expected);
}

TEST(Stage13, ScaleAndShape) {
  Solution solution;
  int n = 100000;
  std::vector<int> p(n);
  std::iota(p.begin(), p.end(), -1);
  std::vector<long long> a(n, 1), expected;
  std::vector<std::array<long long, 3>> ops;
  for (int i = 0; i < 10000; ++i) {
    ops.push_back({0, n - 1, -1LL * i});
    ops.push_back({1, 0, 0});
    expected.push_back(-i);
  }
  EXPECT_EQ(solution.subtreeMin(p, a, ops), expected);
}
