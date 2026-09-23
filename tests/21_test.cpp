#include <gtest/gtest.h>
import std;
import tree21;

TEST(Stage21, ExamplesAndIndependentCalls) {
  Solution s;
  EXPECT_EQ(s.directorySum({10, 20, 30, 40}, {{0, 1, 0},
                                              {0, 2, 1},
                                              {3, 0, 0},
                                              {3, 1, 0},
                                              {0, 1, 3},
                                              {3, 3, 0},
                                              {1, 1, 0},
                                              {0, 1, 3},
                                              {2, 2, 5},
                                              {3, 3, 0}}),
            (std::vector<long long>{60, 50, 40, 65}));
  EXPECT_EQ(s.directorySum(
                {1}, {{0, 0, 0}, {1, 0, 0}, {3, 0, 0}, {2, 0, 0}, {3, 0, 0}}),
            (std::vector<long long>{1, 0}));
  EXPECT_TRUE(s.directorySum({1, 2}, {}).empty());
  EXPECT_TRUE(s.directorySum({1, 2}, {{0, 1, 0}, {1, 1, 0}}).empty());
}

TEST(Stage21, RandomParentOracle) {
  Solution s;
  std::mt19937 rng(7121);
  for (int n : {2, 9, 30}) {
    std::vector<long long> initial(n), values;
    for (auto &v : initial)
      v = static_cast<int>(rng() % 21);
    values = initial;
    std::vector<int> parent(n, -1);
    auto root = [&](int u) {
      while (parent[u] != -1)
        u = parent[u];
      return u;
    };
    std::vector<std::array<long long, 3>> ops;
    std::vector<long long> expected;
    auto add = [&](int t, int u, long long v) {
      ops.push_back({t, u, v});
      if (t == 0) {
        if (parent[u] == -1 && root(u) != root(v))
          parent[u] = v;
      } else if (t == 1)
        parent[u] = -1;
      else if (t == 2)
        values[u] = v;
      else {
        long long sum = 0;
        for (int x = 0; x < n; ++x)
          for (int a = x; a != -1; a = parent[a])
            if (a == u) {
              sum += values[x];
              break;
            }
        expected.push_back(sum);
      }
    };
    add(0, 1, 0);
    add(0, 0, 1);
    add(3, 0, 0);
    add(1, 1, 0);
    add(1, 1, 0);
    for (int i = 0; i < 2000; ++i) {
      int t = rng() % 4, u = rng() % n;
      long long v = t == 0   ? rng() % n
                    : t == 2 ? static_cast<int>(rng() % 31)
                             : 0;
      add(t, u, v);
    }
    EXPECT_EQ(s.directorySum(initial, ops), expected);
  }
}

TEST(Stage21, ChainAndStar) {
  constexpr int n = 30000;
  Solution s;
  for (bool star : {false, true}) {
    std::vector<std::array<long long, 3>> ops;
    std::vector<long long> expected;
    for (int i = 1; i < n; ++i)
      ops.push_back({0, i, star ? 0 : i - 1});
    for (int i = 0; i < 200; ++i) {
      ops.push_back({3, 0, 0});
      expected.push_back(n * 1000000000LL);
      ops.push_back({1, 1, 0});
      ops.push_back({3, 0, 0});
      expected.push_back((star ? n - 1LL : 1LL) * 1000000000LL);
      ops.push_back({3, 1, 0});
      expected.push_back((star ? 1LL : n - 1LL) * 1000000000LL);
      ops.push_back({0, 1, 0});
    }
    EXPECT_EQ(s.directorySum(std::vector<long long>(n, 1000000000LL), ops),
              expected);
  }
}
