#include <gtest/gtest.h>
import std;
import tree02;
#include "oracle.hpp"

TEST(Stage02, SmallOracleAndEdgeCases) {
  Solution solution;
  std::mt19937 rng(7102);
  for (int n : {1, 2, 7, 31}) {
    std::vector<long long> a(n);
    for (auto &x : a)
      x = static_cast<int>(rng() % 11) - 5;
    std::vector<std::array<int, 2>> queries;
    for (int l = 0; l < n; ++l)
      for (int r = l; r < n; ++r)
        queries.push_back({l, r});
    std::shuffle(queries.begin(), queries.end(), rng);
    EXPECT_EQ(solution.rangeMin(a, queries), oracle::rangeMin(a, queries));
    EXPECT_TRUE(solution.rangeMin(a, {}).empty());
  }
}

TEST(Stage02, ScaleAndShape) {
  Solution solution;
  const int n = 120000;
  std::vector<long long> a(n);
  std::iota(a.begin(), a.end(), -60000LL);
  std::vector<std::array<int, 2>> queries;
  std::vector<long long> expected;
  for (int i = 0; i < 20000; ++i) {
    int l = (i * 7919LL) % n;
    queries.push_back({l, n - 1});
    expected.push_back(a[l]);
  }
  EXPECT_EQ(solution.rangeMin(a, queries), expected);
}
