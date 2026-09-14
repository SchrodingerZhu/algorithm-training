export module sieve03;
import std;

export class Solution {
public:
    std::vector<int> totients(int n) {
        std::vector<int> leastPrimes(n + 1, 0), totients(n + 1), primes;
        if (n >= 1)
            totients[1] = 1;
        for (int i = 2; i <= n; ++i) {
            if (leastPrimes[i] == 0) {
                leastPrimes[i] = i;
                primes.push_back(i);
                totients[i] = i - 1;
            }
            for (int p : primes) {
                if (p > n / i) break;
                leastPrimes[p * i] = p;
                if (i % p == 0) {
                    // T[p * i] = T[p * p^e * r] 
                    //          = T[p^{e + 1} * r] 
                    //          = T[p^{e + 1}]T[r] 
                    //          = p * T[p^e] * T[r] 
                    //          = p * T[p^e * r] 
                    //          = p * T[i]
                    totients[p * i] = p * totients[i];
                    break;
                }
                totients[p * i] = totients[p] * totients[i];
            }
        }
        return totients;
    }
};
