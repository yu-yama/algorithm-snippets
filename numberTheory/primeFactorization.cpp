#include <iostream>
#include <map>

using namespace std;

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
        number_type ans = n;
        for (const auto& i : factors()) (ans /= i.first) *= i.first - 1;
        return ans;
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
    }
    return 0;
}
