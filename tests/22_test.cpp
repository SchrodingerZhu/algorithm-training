#include <gtest/gtest.h>
import std;
import tree22;
#include "oracle.hpp"

namespace {
long long componentSum(const oracle::Forest &f, int start, int blockedU = -1,
                       int blockedV = -1) {
  std::vector<bool> seen(f.g.size());
  std::vector<int> queue{start};
  seen[start] = true;
  long long sum = 0;
  for (std::size_t i = 0; i < queue.size(); ++i) {
    int u = queue[i];
    sum += f.val[u];
    for (int v : f.g[u]) {
      if ((u == blockedU && v == blockedV) || (u == blockedV && v == blockedU))
        continue;
      if (!seen[v]) {
        seen[v] = true;
        queue.push_back(v);
      }
    }
  }
  return sum;
}
} // namespace

TEST(Stage22, ExamplesAndIndependentCalls) {
  Solution s;
  EXPECT_EQ(s.companyBudget({10, 20, -5, 100}, {{0, 0, 1},
                                                {0, 1, 2},
                                                {0, 1, 3},
                                                {3, 0, 0},
                                                {4, 0, 1},
                                                {4, 1, 0},

                                                {4, 1, 3},
                                                {2, 3, -10},
                                                {4, 0, 1},
                                                {3, 2, 0},
                                                {4, 0, 2}}),
            (std::vector<std::optional<long long>>{125, 115, 10, 100, 5, 15,
                                                   std::nullopt}));
  EXPECT_EQ(s.companyBudget({-1}, {{3, 0, 0}, {4, 0, 0}, {2, 0, 0}, {3, 0, 0}}),
            (std::vector<std::optional<long long>>{-1, std::nullopt, 0}));
  EXPECT_TRUE(s.companyBudget({1, 2}, {}).empty());
  EXPECT_TRUE(
      s.companyBudget({1, 2}, {{0, 0, 1}, {1, 1, 0}, {2, 0, 5}}).empty());
}

TEST(Stage22, RandomForestOracle) {
  Solution s;
  std::mt19937 rng(7122);
  for (int n : {2, 8, 29}) {
    std::vector<long long> values(n);
    for (auto &v : values)
      v = static_cast<int>(rng() % 21) - 10;
    oracle::Forest f(values);
    std::vector<std::array<long long, 3>> ops;
    std::vector<std::optional<long long>> expected;
    auto add = [&](int t, int u, long long v) {
      ops.push_back({t, u, v});
      if (t == 0)
        f.link(u, v);
      else if (t == 1)
        f.cut(u, v);
      else if (t == 2)
        f.val[u] = v;
      else if (t == 3)
        expected.push_back(componentSum(f, u));
      else if (t == 4) {
        if (!f.g[u].contains(v))
          expected.push_back(std::nullopt);
        else
          expected.push_back(componentSum(f, v, u, v));
      }
    };
    for (int i = 1; i < n; ++i)
      add(0, i - 1, i);
    for (int i = 0; i < 2500; ++i) {
      int t = rng() % 5, u = rng() % n;
      long long v = (t == 3) ? 0
                    : t == 2 ? static_cast<int>(rng() % 31) - 15
                             : rng() % n;
      add(t, u, v);
      if (i % 17 == 0) {
        // Query both sides of an existing edge, then verify the forest is
        // intact.
        for (int a = 0; a < n; ++a)
          if (!f.g[a].empty()) {
            int b = *f.g[a].begin();
            add(4, a, b);

            add(4, b, a);
            add(3, a, 0);
            break;
          }
      }
    }
    EXPECT_EQ(s.companyBudget(values, ops), expected);
  }
}

TEST(Stage22, LargeSumsAndRestoration) {
  constexpr int n = 30000, mid = n / 2;
  std::vector<std::array<long long, 3>> ops;
  std::vector<std::optional<long long>> expected;
  for (int i = 1; i < n; ++i)
    ops.push_back({0, i - 1, i});
  for (int i = 0; i < 1000; ++i) {

    ops.push_back({4, mid - 1, mid});
    expected.push_back((n - mid) * 1000000000LL);
    ops.push_back({4, mid, mid - 1});
    expected.push_back(mid * 1000000000LL);
    ops.push_back({3, n - 1, 0});
    expected.push_back(n * 1000000000LL);
    ops.push_back({1, mid - 1, mid});
    ops.push_back({4, mid - 1, mid});
    expected.push_back(std::nullopt);
    ops.push_back({0, mid, mid - 1});
  }
  EXPECT_EQ(
      Solution{}.companyBudget(std::vector<long long>(n, 1000000000LL), ops),
      expected);
}
