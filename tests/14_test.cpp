#include <gtest/gtest.h>
import std;
import tree14;
#include "oracle.hpp"

TEST(Stage14, SmallOracleAndEdgeCases) {
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
    int u = rng() % 23, v = rng() % 23;
    long long value = static_cast<int>(rng() % 31) - 15;
    if (i % 2 == 0) {
      ops.push_back({0, u, value});
      a[u] = value;
    } else {
      long long best = std::numeric_limits<long long>::max();
      ops.push_back({1, u, v});
      for (int x : oracle::path(p, u, v))
        best = std::min(best, a[x]);
      expected.push_back(best);
    }
  }
  EXPECT_EQ(solution.pathMinUpdates(p, initial, ops), expected);
}

TEST(Stage14, ScaleAndShape) {
  Solution solution;
  int n = 100000;
  std::vector<int> p(n);
  std::iota(p.begin(), p.end(), -1);
  std::vector<long long> a(n, 1), expected;
  std::vector<std::array<long long, 3>> ops;
  for (int i = 0; i < 10000; ++i) {
    ops.push_back({0, n - 1, -1LL * i});
    ops.push_back({1, 0, n - 1});
    expected.push_back(-i);
  }
  EXPECT_EQ(solution.pathMinUpdates(p, a, ops), expected);
}
