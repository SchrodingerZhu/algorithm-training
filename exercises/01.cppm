module;
export module sieve01;
import std;

export class Solution {
public:
    // 返回 lp[0..n]；lp[0] = lp[1] = 0，其余为最小质因子。
    std::vector<int> leastPrimeFactors(int n) {
        std::vector<int> leastPrimes(n + 1, 0), primes; 
        if (n < 2)
            return leastPrimes;
        for (int num : std::views::iota(2, n + 1)) {
            if (leastPrimes[num] == 0) {
                leastPrimes[num] = num;
                primes.push_back(num);
            }
            for (int p : primes) {
                if (p > n / num) break;
                leastPrimes[p * num] = p;
                if (num % p == 0) break;
            }
        }
        return leastPrimes;
    }
};
