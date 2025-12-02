#include "Common.hpp"

bool PrimeUtils::isPrime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

int PrimeUtils::getNextPrime(int n) {
    if (n < 2) return 2;
    while (!isPrime(n)) n++;
    return n;
}

int PrimeUtils::getNextPrimeGreater(int n) {
    return getNextPrime(n + 1);
}

int PrimeUtils::getNextSizeForRehash(int n) {
    int next_size = n * 2;
    return getNextPrime(next_size);
}
