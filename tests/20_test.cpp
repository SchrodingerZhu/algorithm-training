#include <gtest/gtest.h>
import std;
import tree20;
#include "oracle.hpp"

namespace {
long long total(const oracle::Forest &f, int start) {
  std::vector<bool> seen(f.g.size());
  std::vector<int> queue{start};
  seen[start] = true;
  long long sum = 0;
  for (std::size_t i = 0; i < queue.size(); ++i) {
    int u = queue[i];
    sum += f.val[u];
    for (int v : f.g[u])
      if (!seen[v]) {
        seen[v] = true;
        queue.push_back(v);
      }
  }
  return sum;
}
} // namespace

TEST(Stage20, ExamplesAndIndependentCalls) {
  Solution s;
  EXPECT_EQ(s.componentSum({2, -1, 4, 100}, {{0, 0, 1},
                                             {0, 1, 2},
                                             {0, 1, 3},
                                             {3, 0, 0},
                                             {2, 1, 5},
                                             {3, 2, 0},
                                             {1, 1, 3},
                                             {3, 0, 0},
                                             {3, 3, 0}}),
            (std::vector<long long>{105, 111, 11, 100}));
  EXPECT_EQ(s.componentSum({-1}, {{3, 0, 0}, {2, 0, 0}, {3, 0, 0}}),
            (std::vector<long long>{-1, 0}));
  EXPECT_TRUE(s.componentSum({1, 2}, {}).empty());
  EXPECT_TRUE(
      s.componentSum({1, 2}, {{0, 0, 1}, {1, 1, 0}, {2, 0, -5}}).empty());
  EXPECT_EQ(
      s.componentSum({1, 2},
                     {{0, 0, 1}, {1, 1, 0}, {2, 1, -7}, {0, 1, 0}, {3, 0, 0}}),
      (std::vector<long long>{-6}));
}

TEST(Stage20, RandomForestOracle) {
  Solution s;
  std::mt19937 rng(7120);
  for (int n : {2, 8, 29}) {
    std::vector<long long> values(n);
    for (auto &v : values)
      v = static_cast<int>(rng() % 21) - 10;
    oracle::Forest f(values);
    std::vector<std::array<long long, 3>> ops;
    std::vector<long long> expected;
    auto add = [&](int t, int u, long long v) {
      ops.push_back({t, u, v});
      if (t == 0)
        f.link(u, v);
      else if (t == 1)
        f.cut(u, v);
      else if (t == 2)
        f.val[u] = v;
      else
        expected.push_back(total(f, u));
    };
    for (int i = 1; i < n; ++i)
      add(0, i - 1, i);
    add(0, 0, n - 1);
    add(1, 0, 0);
    add(3, 0, 0);
    for (int i = 0; i < 2500; ++i) {
      int t = rng() % 4, u = rng() % n;
      long long v = 0;
      if (t < 2)
        v = rng() % n;
      if (t == 2)
        v = static_cast<int>(rng() % 31) - 15;
      add(t, u, v);
    }
    EXPECT_EQ(s.componentSum(values, ops), expected);
  }
}

TEST(Stage20, LargeSumsChainAndStar) {
  constexpr int n = 30000;
  Solution s;
  for (bool star : {false, true}) {
    std::vector<std::array<long long, 3>> ops;
    std::vector<long long> expected;
    for (int i = 1; i < n; ++i)
      ops.push_back({0, star ? 0 : i - 1, i});
    for (int i = 0; i < 1000; ++i) {
      ops.push_back({2, 0, -i});
      ops.push_back({3, n - 1, 0});
      expected.push_back((n - 1) * 1000000000LL - i);
      ops.push_back({1, n - 1, star ? 0 : n - 2});
      ops.push_back({3, n - 1, 0});
      expected.push_back(1000000000LL);
      ops.push_back({3, 0, 0});
      expected.push_back((n - 2) * 1000000000LL - i);
      ops.push_back({0, n - 1, star ? 0 : n - 2});
    }
    EXPECT_EQ(s.componentSum(std::vector<long long>(n, 1000000000LL), ops),
              expected);
  }
}
