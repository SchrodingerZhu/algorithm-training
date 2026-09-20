#include <gtest/gtest.h>
import std;

static_assert(__cplusplus > 202302L, "Use C++26 mode.");

TEST(Environment, StandardLibraryModuleAndGoogleTest) {
    const std::vector<int> values{2, 3, 5, 7};
    EXPECT_EQ(std::accumulate(values.begin(), values.end(), 0), 17);
    EXPECT_EQ(std::format("{} primes", values.size()), "4 primes");
}

namespace {
std::generator<int> generatedValues() {
    co_yield 1;
    co_yield 2;
}
}

TEST(Environment, GeneratorFromStandardLibraryModule) {
    auto values = generatedValues() | std::ranges::to<std::vector>();
    EXPECT_EQ(values, (std::vector<int>{1, 2}));
}
