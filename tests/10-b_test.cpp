#include <gtest/gtest.h>
import std;
import tree10b;

namespace {
long long bruteSumSubarrayMinimums(const std::vector<long long> &values) {
  long long total = 0;
  for (std::size_t l = 0; l < values.size(); ++l) {
    long long minimum = values[l];
    for (std::size_t r = l; r < values.size(); ++r) {
      minimum = std::min(minimum, values[r]);
      total += minimum;
    }
  }
  return total;
}
} // namespace

TEST(Stage10B, ExamplesAndEdgeCases) {
  Solution solution;
  EXPECT_EQ(solution.sumSubarrayMinimums({3, 1, 2}), 9);
  EXPECT_EQ(solution.sumSubarrayMinimums({3, 1, 2, 4}), 17);
  EXPECT_EQ(solution.sumSubarrayMinimums({5}), 5);
  EXPECT_EQ(solution.sumSubarrayMinimums({-5}), -5);
  EXPECT_EQ(solution.sumSubarrayMinimums({0}), 0);
  EXPECT_EQ(solution.sumSubarrayMinimums({2, 2, 2}), 12);
  EXPECT_EQ(solution.sumSubarrayMinimums({1, 2, 1}), 7);
  EXPECT_EQ(solution.sumSubarrayMinimums({-2, 5, -2}), -5);
  EXPECT_EQ(solution.sumSubarrayMinimums({0, 0, 0}), 0);
}

TEST(Stage10B, SmallOracle) {
  Solution solution;
  std::mt19937 rng(71032);
  for (int trial = 0; trial < 300; ++trial) {
    std::vector<long long> values(1 + rng() % 64);
    for (auto &value : values)
      value = static_cast<int>(rng() % 11) - 5;
    SCOPED_TRACE(trial);
    EXPECT_EQ(solution.sumSubarrayMinimums(values),
              bruteSumSubarrayMinimums(values));
  }
}

TEST(Stage10B, ScaleShapesAndWideResult) {
  Solution solution;
  const int n = 200000;
  const long long intervalCount = n * (n + 1LL) / 2;
  std::vector<long long> values(n, 1000000);
  EXPECT_EQ(solution.sumSubarrayMinimums(values), 1000000LL * intervalCount);
  std::fill(values.begin(), values.end(), -1000000);
  EXPECT_EQ(solution.sumSubarrayMinimums(values), -1000000LL * intervalCount);

  std::iota(values.begin(), values.end(), 1LL);
  const long long monotoneSum = n * (n + 1LL) * (n + 2LL) / 6;
  EXPECT_EQ(solution.sumSubarrayMinimums(values), monotoneSum);
  std::reverse(values.begin(), values.end());
  EXPECT_EQ(solution.sumSubarrayMinimums(values), monotoneSum);

  for (int i = 0; i < n; ++i)
    values[i] = i % 2 == 0 ? -1 : 1;
  EXPECT_EQ(solution.sumSubarrayMinimums(values), -intervalCount + n);
}
