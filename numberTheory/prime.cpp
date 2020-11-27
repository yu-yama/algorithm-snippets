#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

template<typename number_type>
bool is_prime(number_type n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0) return false;
    for (number_type i = 3; i * i <= n; i += 2) if (n % i == 0) return false;
    return true;
}
template<typename number_type>
vector<number_type> divisors(number_type n) {
    number_type i = 1;
    vector<number_type> res;
    for (; i * i < n; ++i) if (n % i == 0) res.push_back(i), res.push_back(n / i);
    if (i * i == n) res.push_back(i);
    sort(res.begin(), res.end());
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    for (int i = 1; i <= 1000; ++i) {
        if (is_prime(i)) cout << i << '\n';
        for (const auto& j : divisors(i)) cout << j << ' ';
        cout << '\n';
    }
    return 0;
}
