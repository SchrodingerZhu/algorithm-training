#include <gtest/gtest.h>
import std;
import tree18;
#include "oracle.hpp"

TEST(Stage18, SmallOracleAndEdgeCases) {
  Solution solution;
  std::mt19937 rng(7118);
  std::vector<long long> initial{2, -1, 4, 0, -7, 6, 3};
  oracle::Forest f(initial);
  std::vector<std::array<long long, 3>> ops;
  std::vector<std::optional<long long>> expected;
  auto add = [&](int t, int u, long long x) {
    ops.push_back({t, u, x});
    if (t == 0)
      f.link(u, x);
    else if (t == 1)
      f.cut(u, x);
    else if (t == 2)
      f.val[u] = x;
    else
      expected.push_back(f.sum(u, x));
  };
  add(3, 1, 1);
  add(3, 0, 2);
  add(0, 0, 1);
  add(0, 1, 2);
  add(1, 0, 2);
  add(3, 2, 0);
  add(0, 2, 0);
  add(3, 0, 2);
  for (int i = 0; i < 2000; ++i) {
    int t = rng() % 4, u = rng() % initial.size();
    long long x = t == 2 ? static_cast<int>(rng() % 31) - 15
                         : static_cast<int>(rng() % initial.size());
    add(t, u, x);
  }
  EXPECT_EQ(solution.dynamicPathSum(initial, ops), expected);
}

TEST(Stage18, ScaleAndShape) {
  Solution solution;
  int n = 30000;
  std::vector<long long> a(n, 1);
  std::vector<std::array<long long, 3>> ops;
  std::vector<std::optional<long long>> expected;
  for (int i = 1; i < n; ++i)
    ops.push_back({0, i - 1, i});
  for (int i = 0; i < 10000; ++i) {
    ops.push_back({2, 0, i});
    ops.push_back({3, n - 1, 0});
    expected.push_back(n - 1LL + i);
    ops.push_back({1, n / 2 - 1, n / 2});
    ops.push_back({3, 0, n - 1});
    expected.push_back(std::nullopt);
    ops.push_back({0, n / 2 - 1, n / 2});
  }
  EXPECT_EQ(solution.dynamicPathSum(a, ops), expected);
}
