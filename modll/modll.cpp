#include <iostream>
#include <limits>
#include <vector>
#include <chrono>
#define MOD 998244353
#define MODL 29996224275833
#define INF (1 << 30)
#define INFL (1LL << 62)

#define tNow chrono::duration_cast<usec>(chrono::system_clock::now().time_since_epoch())

using namespace std;

using usec = chrono::microseconds;
using ll = long long;
using mod_type = ll;

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
    using number_type = ll;
    constexpr static number_type max_num = numeric_limits<number_type>::max();
    number_type n;
private:
    constexpr void check_negative() noexcept {
        if (n < 0) n += m;
    }
public:
    constexpr Fp(number_type nn = 0) noexcept : n(nn % m) {
        check_negative();
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
        extended_euclidean<ll, Fp>(n, m, u, v);
        return u;
    }
    constexpr Fp& operator/=(const Fp& a) noexcept {
        *this *= a.inv();
        check_negative();
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
        if (!a) return 1;
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

template<class T>
struct BinomialCoefficient {
    using index_type = ll;
    vector<T> fact_, inv_, finv_;
    constexpr BinomialCoefficient() noexcept {}
    constexpr BinomialCoefficient(index_type n) noexcept {
        init(n);
    }
    constexpr void init(const index_type& n) noexcept {
        fact_.assign(n, 1), inv_.assign(n, 1), finv_.assign(n, 1);
        auto m = fact_.front().mod();
        for (index_type i = 2; i < n; ++i) {
            fact_[i] = fact_[i - 1] * i;
            inv_[i] = -inv_[m % i] * (m / i);
            finv_[i] = finv_[i - 1] * inv_[i];
        }
    }
    constexpr T comb(const index_type& n, const index_type& k) const noexcept {
        if (n < k || n < 0 || k < 0) return 0;
        return fact_[n] * finv_[k] * finv_[n - k];
    }
    constexpr T fact(const index_type& n) const noexcept {
        if (n < 0) return 0;
        return fact_[n];
    }
    constexpr T inv(const index_type& n) const noexcept {
        if (n < 0) return 0;
        return inv_[n];
    }
    constexpr T finv(const index_type& n) const noexcept {
        if (n < 0) return 0;
        return finv_[n];
    }
};

template<class T>
struct Stirling {
    using index_type = ll;
    vector< vector<T> > s;
    constexpr Stirling(index_type m) noexcept : s(m, vector<T>(m, 0)) {
        s[0][0] = 1;
        for (index_type n = 1; n < m; ++n) for (index_type k = 1; k <= n; ++k) s[n][k] = s[n - 1][k - 1] + s[n - 1][k] * k;
    }
    constexpr T get(index_type n, index_type k) const noexcept {
        if (n < k || n < 0 || k < 0) return 0;
        return s[n][k];
    }
};

int main() {
    ll x, y;

    BinomialCoefficient< Fp<MOD> > bc(10);
    cout << bc.finv(9) << '\n';
    cout << bc.finv(10) << "\n\n";

    cout << extended_euclidean(111, 30, x, y) << '\n' << x << ", " << y << "\n\n";

    Fp<MOD> d;
    cout << d << "\n\n";

    ll t = Fp<MOD>(2);
    cout << (t - 3) << '\n';
    cout << Fp<MOD>(2).pow(100) << '\n';
    cout << Fp<MOD>(2).pow(Fp<MOD>(100)) << "\n\n";

    Fp<MOD> a(2);
    cout << (++a) << '\n' << (a++) << '\n' << a << '\n' << (--a) << '\n' << (a--) << '\n' << a << "\n\n";

    cout << (a -= 3) << '\n' << (a += 3) << '\n' << (a /= 3) << '\n' << (a *= 3) << "\n\n";

    cout << (a + 1) << '\n' << a << '\n' << (1 + a) << '\n' << a << '\n' << "\n\n";

    cout << (a - 1) << '\n' << a << '\n' << (1 - a) << '\n' << a << '\n' << "\n\n";

    cout << a << '\n' << +a << '\n' << a << '\n' << -a << '\n' << a << '\n' << a.pow(3) << '\n' << a << '\n' << a.pow(100) << '\n' << a << '\n' << a.inv() << '\n' << a << "\n\n";

    return 0;
}
