#include <iostream>
#include <vector>
#include <map>
#include <chrono>
#include <cmath>

using namespace std;

struct EratosthenesFactorization {
    using number_type = int;
    using exponent_type = int;
private:
    vector<number_type> lpf_, primes_;
public:
    EratosthenesFactorization(number_type n) : lpf_(n) {
        primes_.reserve(n / log(n) * 1.2);
        for (number_type i = 2; i < n; ++i) {
            if (lpf_[i] == 0) {
                lpf_[i] = i;
                primes_.push_back(i);
            }
            for (const auto& j : primes_) {
                if (j > lpf_[i] || i * j >= n) break;
                lpf_[i * j] = j;
            }
        }
    }
    const vector<number_type>& lpf() const noexcept {
        return lpf_;
    }
    number_type lpf(number_type n) const noexcept {
        return lpf_[n];
    }
    bool is_prime(number_type n) const noexcept {
        return lpf_[n] == n;
    }
    vector<number_type> factors_v(number_type n) const {
        vector<number_type> res;
        while (n > 1) res.push_back(lpf_[n]), n /= lpf_[n];
        return res;
    }
    map<number_type, exponent_type> factors_m(number_type n) const {
        map<number_type, exponent_type> res;
        while (n > 1) ++res[lpf_[n]], n /= lpf_[n];
        return res;
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
    constexpr int MAX = 10000;
    usec st = tNow();
    EratosthenesFactorization sieve(MAX);
    cerr << (tNow() - st).count() << '\n';
    st = tNow();
    for (const auto& i : sieve.primes()) cout << i << '\n';
    for (int i = 0; i < MAX; ++i) cout << sieve.lpf(i) << '\n';
    for (int i = 0; i < MAX; ++i) {
        cout << i << " =";
        for (const auto& j : sieve.factors_v(i)) cout << ' ' << j;
        cout << '\n';
    }
    for (int i = 0; i < MAX; ++i) {
        cout << i << " =";
        for (const auto& j : sieve.factors_m(i)) cout << ' ' << j.first << '^' << j.second;
        cout << '\n';
    }
    cerr << (tNow() - st).count() << '\n';
    return 0;
}
