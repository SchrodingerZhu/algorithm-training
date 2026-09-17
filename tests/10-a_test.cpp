#include <gtest/gtest.h>
import std;
import tree10a;

namespace {
long long bruteLargestRectangle(const std::vector<long long> &heights) {
  long long best = 0;
  for (std::size_t l = 0; l < heights.size(); ++l) {
    long long minimum = heights[l];
    for (std::size_t r = l; r < heights.size(); ++r) {
      minimum = std::min(minimum, heights[r]);
      best = std::max(best, minimum * static_cast<long long>(r - l + 1));
    }
  }
  return best;
}
} // namespace

TEST(Stage10A, ExamplesAndEdgeCases) {
  Solution solution;
  EXPECT_EQ(solution.largestRectangle({2, 1, 5, 6, 2, 3}), 10);
  EXPECT_EQ(solution.largestRectangle({7}), 7);
  EXPECT_EQ(solution.largestRectangle({0}), 0);
  EXPECT_EQ(solution.largestRectangle({2, 2, 2}), 6);
  EXPECT_EQ(solution.largestRectangle({2, 0, 2}), 2);
  EXPECT_EQ(solution.largestRectangle({2, 3, 2}), 6);
  EXPECT_EQ(solution.largestRectangle({0, 0, 0}), 0);
}

TEST(Stage10A, SmallOracle) {
  Solution solution;
  std::mt19937 rng(71031);
  for (int trial = 0; trial < 300; ++trial) {
    std::vector<long long> heights(1 + rng() % 64);
    for (auto &height : heights)
      height = rng() % 11;
    SCOPED_TRACE(trial);
    EXPECT_EQ(solution.largestRectangle(heights), bruteLargestRectangle(heights));
  }
}

TEST(Stage10A, ScaleShapesAndWideResult) {
  Solution solution;
  const int n = 200000;
  std::vector<long long> heights(n, 1000000000LL);
  EXPECT_EQ(solution.largestRectangle(heights), 1000000000LL * n);

  std::iota(heights.begin(), heights.end(), 1LL);
  const long long monotoneArea = (n + 1LL) * (n + 1LL) / 4;
  EXPECT_EQ(solution.largestRectangle(heights), monotoneArea);
  std::reverse(heights.begin(), heights.end());
  EXPECT_EQ(solution.largestRectangle(heights), monotoneArea);

  for (int i = 0; i < n; ++i)
    heights[i] = i % 2 == 0 ? 1000000000LL : 0;
  EXPECT_EQ(solution.largestRectangle(heights), 1000000000LL);
}
