#include <iostream>
#include <limits>

using namespace std;

using mod_type = long long;

template<typename T, typename U>
constexpr T extended_euclidean(const T& a, const T& b, U& x, U& y) noexcept {
    if (!b) {
        x = 1, y = 0;
        return a;
    }
    T t = a / b, r = extended_euclidean(b, a - b * t, y, x);
    y -= x * t;
    return r;
}

template<mod_type m>
struct Fp {
    using number_type = long long;
    constexpr static number_type max_num = numeric_limits<number_type>::max();
private:
    number_type n;
    constexpr void fix_negative() noexcept {
        if (n < 0) n += m;
    }
public:
    constexpr Fp(number_type nn = 0) noexcept : n(nn % m) {
        fix_negative();
    }
    constexpr mod_type mod() const noexcept {
        return m;
    }
    template<typename T> constexpr operator T() const {
        return T(n);
    }
    constexpr Fp operator+() const noexcept {
        return *this;
    }
    constexpr Fp operator-() const noexcept {
        return n ? m - n : 0;
    }
    constexpr Fp& operator++() noexcept {
        return *this += 1;
    }
    constexpr Fp operator++(int) noexcept {
        Fp t(*this);
        ++(*this);
        return t;
    }
    constexpr Fp& operator--() noexcept {
        return *this -= 1;
    }
    constexpr Fp operator--(int) noexcept {
        Fp t(*this);
        --(*this);
        return t;
    }
    constexpr Fp& operator+=(const Fp& a) noexcept {
        if ((n += a.n) >= m) n -= m;
        return *this;
    }
    constexpr Fp& operator-=(const Fp& a) noexcept {
        return *this += -a;
    }
    constexpr Fp& operator*=(const Fp& a) noexcept {
        if (!a) return *this = 0;
        if (n <= max_num / a.n) {
            (n *= a.n) %= m;
        } else {
            Fp t(*this);
            (t += t) *= Fp(a.n >> 1);
            if (a.n & 1) *this += t;
            else *this = t;
        }
        return *this;
    }
    constexpr Fp inv() const noexcept {
        Fp u, v;
        extended_euclidean<long long, Fp>(n, m, u, v);
        return u;
    }
    constexpr Fp& operator/=(const Fp& a) noexcept {
        *this *= a.inv();
        fix_negative();
        return *this;
    }
    constexpr Fp operator+(const Fp& a) const noexcept {
        return Fp(*this) += a;
    }
    template<typename T> friend constexpr Fp operator+(const T& a, const Fp& b) noexcept {
        return b + a;
    }
    constexpr Fp operator-(const Fp& a) const noexcept {
        return Fp(*this) -= a;
    }
    template<typename T> friend constexpr Fp operator-(const T& a, const Fp& b) noexcept {
        return -(b - a);
    }
    constexpr Fp operator*(const Fp& a) const noexcept {
        return Fp(*this) *= a;
    }
    template<typename T> friend constexpr Fp operator*(const T& a, const Fp& b) noexcept {
        return b * a;
    }
    constexpr Fp operator/(const Fp& a) const noexcept {
        return Fp(*this) /= a;
    }
    template<typename T> friend constexpr Fp operator/(const T& a, const Fp& b) noexcept {
        return (b / a).inv();
    }
    constexpr bool operator==(const Fp& a) const noexcept {
        return n == a.n;
    }
    constexpr bool operator!=(const Fp& a) const noexcept {
        return n != a.n;
    }
    constexpr Fp pow(const number_type& a) const noexcept {
        if (!a) return Fp(1);
        if (a < 0) return inv().pow(-a);
        Fp t = pow(a >> 1);
        t *= t;
        if (a & 1) t *= *this;
        return t;
    }
    friend constexpr istream& operator>>(istream& s, Fp& a) {
        return s >> a.n;
    }
    friend constexpr ostream& operator<<(ostream& s, const Fp& a) {
        return s << a.n;
    }
};

template<typename T>
constexpr pair<T, T> fibonacci(unsigned n) {
    if (n == 0) return {0, 1};
    auto [a, b] = fibonacci<T>(n >> 1);
    T c = a * (2 * b - a), d = a * a + b * b;
    if (n & 1) return {d, c + d};
    else return {c, d};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    using ll = long long;
    constexpr int MOD = 1000000007;
    constexpr auto a = fibonacci<int>(10).first;
    static_assert(a == 55);
    constexpr auto b = fibonacci<ll>(50).first;
    static_assert(b == 12586269025);
    constexpr auto c = fibonacci< Fp<MOD> >(100).first;
    static_assert(c == 687995182);
    constexpr auto d = fibonacci< Fp<MOD> >(-1).first;
    static_assert(d == 890281843);
    return 0;
}
