#include <iostream>
#include <numeric>
#include <cmath>
// #include <boost/multiprecision/cpp_int.hpp>

using namespace std;

namespace {
    constexpr long long abs_(const long long& n) {
        return (n < 0 ? -n : n);
    }
}

struct Rational {
    using number_type = long long;
    // using number_type = boost::multiprecision::cpp_int;
private:
    number_type n, d;
    constexpr void check_zero() const {
        if (d == 0) throw std::invalid_argument("zero assigned as a denominator");
    }
    constexpr Rational& reduce() {
        check_zero();
        if (d < 0) n = -n, d = -d;
        number_type g = gcd(abs_(n), d);
        n /= g, d /= g;
        return *this;
    }
    constexpr Rational& reduce(const Rational& a) {
        number_type l = lcm(d, a.d);
        n *= l / d, d = l;
        return *this;
    }
public:
    static bool integral_input; // set Rational::integral_input to true to receive input as integers
    constexpr Rational(number_type nn = 0, number_type dd = 1) : n(nn), d(dd) {
        reduce();
    }
    constexpr void set(const number_type& nn, const number_type& dd) {
        *this = Rational(nn, dd);
    }
    constexpr number_type numerator() const noexcept {
        return n;
    }
    constexpr number_type denominator() const noexcept {
        return d;
    }
    constexpr Rational reciprocal() const {
        Rational t(d, n);
        t.reduce();
        return t;
    }
    constexpr Rational operator+() const noexcept {
        return *this;
    }
    constexpr Rational operator-() const noexcept {
        Rational t(*this);
        t.n = -t.n;
        return t;
    }
    constexpr Rational& operator++() {
        return *this += 1;
    }
    constexpr Rational operator++(int) {
        Rational t(*this);
        ++(*this);
        return t;
    }
    constexpr Rational& operator--() {
        return *this -= 1;
    }
    constexpr Rational operator--(int) {
        Rational t(*this);
        --(*this);
        return t;
    }
    constexpr Rational& operator+=(const Rational& a) {
        reduce(a);
        n += a.n * (d / a.d);
        reduce();
        return *this;
    }
    constexpr Rational& operator-=(const Rational& a) {
        *this += -a;
        return *this;
    }
    constexpr Rational& operator*=(const Rational& a) {
        n *= a.n, d *= a.d;
        reduce();
        return *this;
    }
    constexpr Rational& operator/=(const Rational& a) {
        return *this *= a.reciprocal();
    }
    constexpr Rational operator+(const Rational& a) const {
        return Rational(*this) += a;
    }
    constexpr Rational operator-(const Rational& a) const {
        return Rational(*this) -= a;
    }
    constexpr Rational operator*(const Rational& a) const {
        return Rational(*this) *= a;
    }
    constexpr Rational operator/(const Rational& a) const {
        return Rational(*this) /= a;
    }
    constexpr Rational pow(const number_type& a) const {
        if (!a) return 1;
        if (a < 0) return pow(-a).reciprocal();
        Rational t = pow(a >> 1);
        t *= t;
        if (a & 1) t *= *this;
        return t;
    }
    template<typename T> constexpr operator T() const {
        return T(n) / T(d);
    }
    constexpr double sqrt() const {
        return std::sqrt((double)*this);
    }
    constexpr bool operator==(const Rational& a) const noexcept {
        return n == a.n && d == a.d;
    }
    constexpr bool operator!=(const Rational& a) const noexcept {
        return !(*this == a);
    }
    constexpr bool operator<(const Rational& a) const noexcept {
        return n * a.d < a.n * d;
    }
    constexpr bool operator<=(const Rational& a) const noexcept {
        return !(a < *this);
    }
    constexpr bool operator>(const Rational& a) const noexcept {
        return a < *this;
    }
    constexpr bool operator>=(const Rational& a) const noexcept {
        return !(*this < a);
    }
    friend istream& operator>>(istream& s, Rational& a) {
        number_type nn, dd;
        if (Rational::integral_input) s >> nn, a.set(nn, 1);
        else s >> nn >> dd, a.set(nn, dd);
        return s;
    }
    friend ostream& operator<<(ostream& s, const Rational& a) noexcept {
        return s << a.n << '/' << a.d;
    }
};
bool Rational::integral_input = false;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << Rational() << '\n'; // 0/1
    cout << Rational(3) << '\n'; // 3/1
    cout << Rational(-3) << '\n'; // -3/1
    cout << Rational(3, 6) << '\n'; // 1/2
    cout << Rational(-3, 6) << '\n'; // -1/2
    cout << Rational(3, -6) << '\n'; // -1/2
    cout << Rational(-3, -6) << '\n'; // 1/2
    cout << '\n';

    Rational a(3, 5);

    cout << a << '\n'; // 3/5
    cout << a.numerator() << '\n' << a.denominator() << '\n' << a.reciprocal() << '\n'; // 3, 5, 5/3
    cout << +a << '\n' << -a << '\n'; // 3/5, -3/5
    cout << '\n';

    cout << (++a) << '\n' << (a++) << '\n' << a << '\n' << (--a) << '\n' << (a--) << '\n' << a << '\n'; // 3/5, 8/5, 8/5, 13/5, 8/5, 8/5, 3/5
    cout << '\n';

    cout << (a -= 3) << '\n' << (a += 3) << '\n' << (a /= 3) << '\n' << (a *= 3) << '\n'; // -12/5, 3/5, 1/5, 3/5;
    cout << '\n';

    cout << (a - 3) << '\n' << (a + 3) << '\n' << (a / 3) << '\n' << (a * 3) << '\n'; // -12/5, 18/5, 1/5, 9/5;
    cout << '\n';

    cout << (a -= Rational(4, 15)) << '\n' << (a += Rational(4, 15)) << '\n' << (a /= Rational(4, 15)) << '\n' << (a *= Rational(4, 15)) << '\n'; // 1/3, 3/5, 9/4, 3/5;
    cout << '\n';

    cout << (a - Rational(4, 15)) << '\n' << (a + Rational(4, 15)) << '\n' << (a / Rational(4, 15)) << '\n' << (a * Rational(4, 15)) << '\n'; // 1/3, 13/15, 9/4, 4/25;
    cout << '\n';

    cout << a.pow(0) << '\n' << a.pow(3) << '\n' << a.pow(-3) << '\n'; // 1, 27/125, 125/27
    cout << '\n';

    cout << (int)a << '\n' << (long long)a << '\n' << (float)a << '\n' << (double)a << '\n'; // 0, 0, 0.6, 0.6
    cout << '\n';

    cout << a.sqrt() << '\n'; // 0.774597
    cout << '\n';

    Rational b(6, 10);
    cout << (a == b) << '\n' << (a != b) << '\n' << (a < b) << '\n' << (a <= b) << '\n' << (a > b) << '\n' << (a >= b) << '\n'; // 1, 0, 0, 1, 0, 1
    cout << '\n';

    Rational c(2, 3);
    cout << (a == c) << '\n' << (a != c) << '\n' << (a < c) << '\n' << (a <= c) << '\n' << (a > c) << '\n' << (a >= c) << '\n'; // 0, 1, 1, 1, 0, 0
    cout << '\n';

    Rational::integral_input = true;
    cin >> a;
    cout << a << '\n'; // [input]/1
    cout << '\n';

    Rational::integral_input = false;
    cin >> a;
    cout << a << '\n'; // [input 1]/[input 2]
    cout << '\n';

    return 0;
}
