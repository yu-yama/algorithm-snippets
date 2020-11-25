#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

using namespace std;

struct Eratosthenes {
    using number_type = int;
    using size_type = vector<number_type>::size_type;
private:
    vector<bool> is_prime_;
    vector<number_type> primes_;
public:
    Eratosthenes(size_type n) : is_prime_(n, true) {
        if (n <= 0) return;
        primes_.reserve(n / log(n) * 1.2);
        is_prime_[0] = (is_prime_[1] = false);
        for (size_type i = 2; i < n; ++i) if (is_prime_[i]) {
            primes_.push_back(i);
            for (size_type j = i * i; j < n; j += i) is_prime_[j] = false;
        }
    }
    const vector<bool>& is_prime() const noexcept {
        return is_prime_;
    }
    bool is_prime(size_type n) const noexcept {
        return is_prime_[n];
    }
    const vector<number_type>& primes() const noexcept {
        return primes_;
    }
};

struct SegmentedEratosthenes {
    using number_type = int;
    using size_type = vector<number_type>::size_type;
private:
    number_type s;
    Eratosthenes sieve;
    vector<bool> is_prime_;
    vector<number_type> primes_;
public:
    SegmentedEratosthenes(number_type l, number_type r) : s(sqrt(r) * 1.1), sieve(s), is_prime_(r - l, true) {
        for (const auto& i : sieve.primes()) for (number_type j = max(i * i, (l + i - 1) / i * i); j < r; j += i) is_prime_[j - l] = false;
        if (l == 1) is_prime_[0] = false;
        primes_.reserve((r / log(r) - l / log(l)) * 1.2);
        for (number_type i = l; i < r; ++i) if (is_prime_[i - l]) primes_.push_back(i);
    }
    const vector<bool>& is_prime() const noexcept {
        return is_prime_;
    }
    bool is_prime(size_type n) const noexcept {
        return is_prime_[n];
    }
    const vector<number_type>& primes() const noexcept {
        return primes_;
    }
};

namespace {
    using usec = chrono::microseconds;
    usec tNow() {
        return chrono::duration_cast<usec>(chrono::system_clock::now().time_since_epoch());
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    usec st = tNow();
    Eratosthenes sieve(1000000);
    cerr << (tNow() - st).count() << '\n';
    st = tNow();
    for (const auto& i : sieve.primes()) cout << i << '\n';
    cerr << (tNow() - st).count() << '\n';
    st = tNow();
    SegmentedEratosthenes ssieve(99000000, 100000000);
    cerr << (tNow() - st).count() << '\n';
    st = tNow();
    for (const auto& i : ssieve.primes()) cout << i << '\n';
    cerr << (tNow() - st).count() << '\n';
    return 0;
}
