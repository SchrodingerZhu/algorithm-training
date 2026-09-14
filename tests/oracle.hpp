#pragma once

// 小规模独立判定器：用试除／枚举核对筛法，不作为解题模板。
inline std::vector<std::pair<int, int>> factorize(int x) {
    std::vector<std::pair<int, int>> result;
    for (int p = 2; p <= x / p; ++p) {
        int exponent = 0;
        while (x % p == 0) { x /= p; ++exponent; }
        if (exponent) result.emplace_back(p, exponent);
    }
    if (x > 1) result.emplace_back(x, 1);
    return result;
}

inline int trialLeastPrime(int x) {
    if (x < 2) return 0;
    for (int p = 2; p <= x / p; ++p) if (x % p == 0) return p;
    return x;
}

inline std::array<int, 2> trialCounts(int x) {
    auto factors = factorize(x);
    int total = 0;
    for (auto [p, exponent] : factors) { (void)p; total += exponent; }
    return {static_cast<int>(factors.size()), total};
}

inline int brutePhi(int x) {
    int result = 0;
    for (int y = 1; y <= x; ++y) result += std::gcd(x, y) == 1;
    return result;
}

inline std::array<long long, 2> bruteDivisors(int x) {
    std::array<long long, 2> result{};
    for (int d = 1; d <= x / d; ++d) if (x % d == 0) {
        ++result[0]; result[1] += d;
        if (d != x / d) { ++result[0]; result[1] += x / d; }
    }
    return result;
}

inline std::vector<int> mobiusByDivisorIdentity(int n) {
    std::vector<int> mu(n + 1);
    if (n) mu[1] = 1;
    for (int d = 1; d <= n; ++d)
        for (int multiple = 2 * d; multiple <= n; multiple += d)
            mu[multiple] -= mu[d];
    return mu;
}

inline long long bruteCoprimePairs(int n, int m) {
    long long result = 0;
    for (int a = 1; a <= n; ++a)
        for (int b = 1; b <= m; ++b) result += std::gcd(a, b) == 1;
    return result;
}
