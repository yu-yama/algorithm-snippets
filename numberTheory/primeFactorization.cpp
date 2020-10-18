#include <iostream>
#include <map>

using namespace std;

struct PrimeFactorization {
    using number_type = long long;
    using exponent_type = int;
private:
    map<number_type, exponent_type> factors_;
public:
    PrimeFactorization(number_type n) {
        for (number_type i = 2; i * i <= n; ++i) while (n % i == 0) ++factors_[i], n /= i;
        if (n != 1) ++factors_[n];
    }
    const auto& factors() const {
        return factors_;
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
    }
    return 0;
}
