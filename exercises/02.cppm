module;
#include <vector>
export module sieve02;
import std;

export class Solution {
public:
    // result[x] = {不同质因子个数 omega(x), 带重数的个数 Omega(x)}。
    std::vector<std::array<int, 2>> factorCounts(int n) {
        std::vector<int> leastPrimes(n + 1, 0), primes;
        std::vector<std::array<int, 2>> counters(n + 1);
        for (int i = 2; i <= n; ++i) {
            if (leastPrimes[i] == 0) {
                leastPrimes[i] = i;
                primes.push_back(i);
                // prime case:
                counters[i] = {1, 1};
            }
            for (int p : primes) {
                if (p > n / i) break;
                leastPrimes[p * i] = p;
                counters[p * i][1] = counters[i][1] + 1;
                if (i % p == 0) {
                    counters[p * i][0] = counters[i][0];
                    break;
                }
                counters[p * i][0] = counters[i][0] + 1;
            }
        }
        return counters;
    }
};
