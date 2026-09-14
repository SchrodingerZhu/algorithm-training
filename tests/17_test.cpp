#include <gtest/gtest.h>
import std;
import tree17;
#include "oracle.hpp"

TEST(Stage17, SmallOracleAndEdgeCases) {
  Solution solution;
  std::mt19937 rng(7117);
  int n = 17;
  oracle::Forest f(std::vector<long long>(n, 0));
  std::vector<std::array<int, 3>> ops;
  std::vector<int> expected;
  auto add = [&](int t, int u, int v) {
    ops.push_back({t, u, v});
    if (t == 0)
      f.link(u, v);
    else if (t == 1)
      f.cut(u, v);
    else
      expected.push_back(!f.path(u, v).empty());
  };
  add(0, 0, 1);
  add(0, 1, 2);
  add(1, 0, 2);
  add(2, 0, 2);
  add(0, 2, 0);
  add(2, 0, 2);
  add(1, 1, 2);
  add(2, 0, 2);
  add(2, 3, 3);
  for (int i = 0; i < 1500; ++i)
    add(rng() % 3, rng() % n, rng() % n);
  EXPECT_EQ(solution.dynamicConnectivity(n, ops), expected);
}

TEST(Stage17, ScaleAndShape) {
  Solution solution;
  int n = 30000;
  std::vector<std::array<int, 3>> ops;
  std::vector<int> expected;
  for (int i = 1; i < n; ++i)
    ops.push_back({0, i - 1, i});
  for (int i = 0; i < 10000; ++i) {
    ops.push_back({2, 0, n - 1});
    expected.push_back(1);
    ops.push_back({1, n / 2 - 1, n / 2});
    ops.push_back({2, 0, n - 1});
    expected.push_back(0);
    ops.push_back({0, n / 2 - 1, n / 2});
  }
  EXPECT_EQ(solution.dynamicConnectivity(n, ops), expected);
}
