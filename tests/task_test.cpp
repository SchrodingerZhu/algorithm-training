#include <gtest/gtest.h>
import std;
import common.task;

namespace {
task<std::size_t> subtreeSizes(
    std::span<const std::vector<std::size_t>> children,
    std::span<std::size_t> sizes, std::size_t node) {
  std::size_t size = 1;
  for (auto child : children[node])
    size += co_await subtreeSizes(children, sizes, child);
  sizes[node] = size;
  co_return size;
}

struct Lifetime {
  int &live;
  explicit Lifetime(int &live) : live(live) { ++live; }
  ~Lifetime() { --live; }
};

task<int> failingChild(int depth, int &live) {
  Lifetime lifetime(live);
  if (depth == 0)
    throw std::runtime_error("leaf failure");
  co_return co_await failingChild(depth - 1, live);
}

task<std::unique_ptr<int>> makeValue() {
  co_return std::make_unique<int>(42);
}

task<int> readValue() {
  auto value = co_await makeValue();
  co_return *value;
}
}

TEST(CommonTask, LazyStartAndSubtreeResults) {
  std::vector<std::vector<std::size_t>> children{{1, 2}, {3}, {}, {}};
  std::vector<std::size_t> sizes(4);
  auto pending = subtreeSizes(children, sizes, 0);
  EXPECT_EQ(sizes, (std::vector<std::size_t>{0, 0, 0, 0}));
  auto moved = std::move(pending);
  EXPECT_EQ(std::move(moved).run(), 4);
  EXPECT_EQ(sizes, (std::vector<std::size_t>{4, 2, 1, 1}));
}

TEST(CommonTask, DeepChain) {
  std::size_t n = 100000;
  std::vector<std::vector<std::size_t>> children(n);
  for (std::size_t i = 1; i < n; ++i)
    children[i - 1].push_back(i);
  std::vector<std::size_t> sizes(n);
  EXPECT_EQ(subtreeSizes(children, sizes, 0).run(), n);
  for (std::size_t i = 0; i < n; ++i)
    ASSERT_EQ(sizes[i], n - i);
}

TEST(CommonTask, ExceptionsUnwindChildFrames) {
  int live = 0;
  EXPECT_THROW(failingChild(1000, live).run(), std::runtime_error);
  EXPECT_EQ(live, 0);
}

TEST(CommonTask, MoveOnlyResult) {
  EXPECT_EQ(readValue().run(), 42);
}
