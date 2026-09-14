module;
#include <array>
export module sieve04;
import std;

export class Solution {
public:
    // tau = Prod_i (e_i + 1)
    // result[x] = {正约数个数 tau(x), 正约数和 sigma(x)}。
    std::vector<std::array<long long, 2>> divisorStats(int n) {
        std::vector<int> leastPrimes(n + 1, 0), primes;
        std::vector<int> power(n + 1, 0);
        std::vector<long long> core(n + 1, 0);
        std::vector<long long> largestPower(n + 1, 0);
        std::vector<long long> sumOfPowers(n + 1, 0);
        std::vector<std::array<long long, 2>> stats(n + 1);
        if (n >= 1)
            stats[1] = {1, 1};
        for (int i = 2; i <= n; ++i) {
            if (leastPrimes[i] == 0) {
                leastPrimes[i] = i;
                primes.push_back(i);
                core[i] = 1;
                power[i] = 1;
                largestPower[i] = i;
                sumOfPowers[i] = 1 + i;
                stats[i] = {2, 1 + i};
            }
            for (int p : primes) {
                if (p > n / i) break;
                leastPrimes[p * i] = p;
                if (i % p == 0) {
                    core[i * p] = core[i];
                    power[i * p] = power[i] + 1;
                    largestPower[i * p] = largestPower[i] * p;
                    sumOfPowers[i * p] = sumOfPowers[i] + largestPower[i * p];
                    stats[i * p][0] = stats[i][0] / (power[i] + 1) * (power[i * p] + 1);
                    stats[i * p][1] = stats[i][1] / sumOfPowers[i] * sumOfPowers[i * p];
                    break;
                }
                core[i * p] = i;
                power[i * p] = 1;
                largestPower[i * p] = p;
                sumOfPowers[i * p] = 1 + p;
                stats[i * p][0] = 2 * stats[i][0];
                stats[i * p][1] = (1 + p) * stats[i][1];  
            }
        }
        return stats;
    }
};
