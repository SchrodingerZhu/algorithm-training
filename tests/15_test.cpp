#include <gtest/gtest.h>
import std;
import tree15;
#include "oracle.hpp"

TEST(Stage15, SmallOracleAndEdgeCases) {
  Solution solution;
  std::mt19937 rng(7113);
  auto p = oracle::parentTree(23, rng);
  std::vector<long long> initial(23), a;
  for (auto &x : initial)
    x = static_cast<int>(rng() % 21) - 10;
  a = initial;
  std::vector<std::array<long long, 4>> ops;
  std::vector<long long> expected;
  for (int i = 0; i < 400; ++i) {
    int u = rng() % 23, v = rng() % 23;
    long long value = static_cast<int>(rng() % 31) - 15;
    auto path = oracle::path(p, u, v);
    if (i % 2 == 0) {
      ops.push_back({0, u, v, value});
      for (int x : path)
        a[x] += value;
    } else {
      ops.push_back({1, u, v, 0});
      long long sum = 0;
      for (int x : path)
        sum += a[x];
      expected.push_back(sum);
    }
  }
  EXPECT_EQ(solution.pathAddSum(p, initial, ops), expected);
}

TEST(Stage15, ScaleAndShape) {
  Solution solution;
  int n = 100000;
  std::vector<int> p(n);
  std::iota(p.begin(), p.end(), -1);
  std::vector<long long> a(n, 1), expected;
  std::vector<std::array<long long, 4>> ops;
  for (int i = 0; i < 10000; ++i) {
    ops.push_back({0, 0, n - 1, 1000000});
    ops.push_back({1, 0, n - 1, 0});
    expected.push_back(1LL * n * (1 + 1000000LL * (i + 1)));
  }
  EXPECT_EQ(solution.pathAddSum(p, a, ops), expected);
}
