#include <iostream>
#include <cmath>
#include <map>
#include <cassert>

using namespace std;

template<typename number_type, typename exponent_type>
constexpr number_type pow_(number_type n, exponent_type a) {
    if constexpr (std::is_integral<exponent_type>::value) {
        if (a < 0) return 0;
        if (a == 0) return 1;
        number_type res = pow_(n, a >> 1);
        res *= res;
        if (a & 1) res *= n;
        return res;
    } else {
        return std::pow(n, a);
    }
}

struct PrimeFactorization {
    using number_type = long long;
    using exponent_type = int;
private:
    map<number_type, exponent_type> factors_;
    number_type n;
public:
    PrimeFactorization(number_type nn) : n(nn) {
        for (number_type i = 2; i * i <= nn; ++i) while (nn % i == 0) ++factors_[i], nn /= i;
        if (nn != 1) ++factors_[nn];
    }
    const auto& factors() const {
        return factors_;
    }
    number_type euler_totient() const {
        number_type res = n;
        for (const auto& i : factors()) (res /= i.first) *= i.first - 1;
        return res;
    }
    exponent_type num_of_divisors() const {
        exponent_type res = 1;
        for (const auto& i : factors()) res *= i.second + 1;
        return res;
    }
    number_type sum_of_divisors() const {
        number_type res = 1;
        for (const auto& i : factors()) res *= (pow_(i.first, i.second + 1) - 1) / (i.first - 1);
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    for (int i = 1; i <= 100000; ++i) {
        PrimeFactorization a(i);
        cout << i << ':';
        for (const auto& j : a.factors()) cout << ' ' << j.first << '^' << j.second;
        cout << '\n';
        cout << "φ(" << i << ") = " << a.euler_totient() << '\n';
        cout << i << " has " << a.num_of_divisors() << " divisor(s), and they sum up to " << a.sum_of_divisors() << ".\n";
    }
    static_assert(pow_(2LL, 62) == (1LL << 62));
    static_assert(pow(3LL, 38) == pow_(3LL, 38));
    assert(typeid(pow(3LL, 38)).name() == typeid(double).name());
    assert(typeid(pow_(3LL, 38)).name() == typeid(long long).name());
    return 0;
}
