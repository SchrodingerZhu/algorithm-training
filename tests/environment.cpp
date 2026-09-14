#include <gtest/gtest.h>
import std;

static_assert(__cplusplus > 202302L, "Use C++26 mode.");

TEST(Environment, StandardLibraryModuleAndGoogleTest) {
    const std::vector<int> values{2, 3, 5, 7};
    EXPECT_EQ(std::accumulate(values.begin(), values.end(), 0), 17);
    EXPECT_EQ(std::format("{} primes", values.size()), "4 primes");
}
