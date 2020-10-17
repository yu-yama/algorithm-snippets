#include <iostream>
#include <limits>
#include <vector>
#include <chrono>

constexpr int MOD = 998244353;
constexpr long long MODL = 29996224275833;
constexpr int INF = (1 << 30);
constexpr long long INFL = (1LL << 62);

#define tNow chrono::duration_cast<usec>(chrono::system_clock::now().time_since_epoch())

using namespace std;

using usec = chrono::microseconds;
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
        extended_euclidean<long long, Fp>(n, m, u, v);
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
    using index_type = long long;
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
    using index_type = long long;
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
    long long x, y;

    BinomialCoefficient< Fp<MOD> > bc(10);
    cout << bc.finv(9) << '\n'; // 712324701 (Fp<mod>(9)!.inv())
    cout << bc.finv(10) << "\n\n"; // undefined behaviour (accessing out of range using operator[])

    cout << extended_euclidean<long long, long long>(111, 30, x, y) << '\n'; // 3 (= gcd(111, 30))
    cout << x << ", " << y << "\n\n"; // 3, -11 (111 * 3 + 30 * (-11) = 3 = gcd(111, 30))

    Fp<MOD> d;
    cout << d << "\n\n"; // 0

    long long t = Fp<MOD>(2);
    cout << (t - 3) << "\n\n"; // -1

    Fp<MOD> a(2);
    cout << (++a) << '\n' << (a++) << '\n' << a << '\n' << (--a) << '\n' << (a--) << '\n' << a << "\n\n"; // 3, 3, 4, 3, 3, 2

    cout << (a -= 3) << '\n' << (a += 3) << '\n' << (a /= 3) << '\n' << (a *= 3) << "\n\n"; // 998244352, 2, 665496236, 2

    cout << (a + 1) << '\n' << a << '\n' << (1 + a) << '\n' << a << '\n' << "\n\n"; // 3, 2, 3, 2

    cout << (a - 1) << '\n' << a << '\n' << (1 - a) << '\n' << a << '\n' << "\n\n"; // 1, 2, 998244352, 2

    cout << +a << '\n' << a << '\n' << -a << '\n' << a << "\n\n"; // 2, 2, 998244351, 2

    cout << a.pow(3) << '\n' << a.pow(100) << '\n' << a.pow(1000000) << '\n' << a << "\n\n"; // 8, 882499718, 421273117

    cout << a.inv() << '\n' << a << "\n\n"; // 499122177, 2

    Fp<MODL> b(INFL);
    cout << b << '\n'; // 6505812270818
    cout << b * INFL << '\n'; // 6130151462911
    cout << (b *= INFL) << "\n\n"; // 6130151462911

    cout << Fp<MODL>(2).pow(122) << '\n'; // 9031593934686
    cout << Fp<MODL>(2).pow(244) << '\n'; // 10412815166631
    cout << Fp<MODL>(2).pow(1000000) << "\n\n"; // 13282742706460

    usec st;

    st = tNow;
    Fp<MODL> basel(1);
    for (int i = 0; i < 1000000; ++i) basel *= 2;
    cout << basel << '\n'; // 13282742706460
    cout << basel.inv() << '\n'; // 605537320149
    cerr << (tNow - st).count() << "\n\n";

    st = tNow;
    Fp<MOD> base(1);
    for (int i = 0; i < 1000000; ++i) base *= 2;
    cout << base << '\n'; // 421273117
    cout << base.inv() << '\n'; // 554214079
    cerr << (tNow - st).count() << '\n';

    return 0;
}
