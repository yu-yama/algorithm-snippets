#include <iostream>
#include <limits>
#include <vector>
#include <chrono>

using namespace std;

namespace {
    struct Barrett {
        using single_int = uint_fast32_t;
        using double_int = uint_fast64_t;
        single_int m;
        double_int im;
        constexpr Barrett(single_int mm) : m(mm), im((double_int)-1 / m + 1) {}
        constexpr static single_int max_mod() {
            return numeric_limits<single_int>::max();
        }
        constexpr single_int umod() const {
            return m;
        }
        constexpr single_int reduce(double_int z) const {
            #ifdef _MSC_VER
                double_int x;
                _umul128(z, im, &x);
            #else
                using quadruple_int = unsigned __int128;
                double_int x = (quadruple_int(z) * im) >> 64;
            #endif
            single_int v = z - x * m;
            if (m <= v) v += m;
            return v;
        }
        constexpr single_int mul(single_int a, single_int b) const {
            double_int z = a;
            z *= b;
            return reduce(z);
        }
    };
}

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

using mod_type = uint_fast64_t;
template<mod_type m>
struct StaticFp {
    using number_type = int_fast64_t;
    static_assert(m >= 2, "mod must be two or greater");
    constexpr static bool is_barrett_safe = m <= static_cast<mod_type>(Barrett::max_mod());
    constexpr static bool use_barrett = false && is_barrett_safe;
    constexpr static Barrett bt{m};
    constexpr static number_type max_num = numeric_limits<number_type>::max();
    static_assert(m <= max_num, "mod must not overflow in number_type");
private:
    number_type n;
    constexpr void fix_negative() noexcept {
        if (n < 0) n += m;
    }
public:
    constexpr StaticFp(number_type nn = 0) noexcept : n(nn % m) {
        fix_negative();
    }
    constexpr static mod_type mod() {
        return m;
    }
    template<typename T> constexpr operator T() const {
        return T(n);
    }
    constexpr StaticFp operator+() const noexcept {
        return *this;
    }
    constexpr StaticFp operator-() const noexcept {
        return n ? m - n : 0;
    }
    constexpr StaticFp& operator++() noexcept {
        return *this += 1;
    }
    constexpr StaticFp operator++(int) noexcept {
        StaticFp t(*this);
        ++(*this);
        return t;
    }
    constexpr StaticFp& operator--() noexcept {
        return *this -= 1;
    }
    constexpr StaticFp operator--(int) noexcept {
        StaticFp t(*this);
        --(*this);
        return t;
    }
    constexpr StaticFp& operator+=(const StaticFp& a) noexcept {
        if ((n += a.n) >= static_cast<number_type>(m)) n -= m;
        return *this;
    }
    constexpr StaticFp& operator-=(const StaticFp& a) noexcept {
        return *this += -a;
    }
    constexpr StaticFp& operator*=(const StaticFp& a) noexcept {
        if constexpr (use_barrett) n = bt.mul(n, a.n);
        else {
            #ifdef _MSC_VER
                if (!a) n = 0;
                else if (n <= max_num / a.n) (n *= a.n) %= m;
                else {
                    StaticFp t(*this);
                    (t += t) *= StaticFp(a.n >> 1);
                    if (a.n & 1) *this += t;
                    else *this = t;
                }
            #else
                if constexpr ((m & (m - 1)) == 0) {
                    // if m is a power of 2
                    n = ((unsigned __int128)n * a.n) & (m - 1);
                } else if constexpr ((m & (m + 1)) == 0) {
                    // if (m + 1) is a power of 2
                    unsigned __int128 t = (unsigned __int128)n * a.n;
                    n = (t >> __builtin_popcountll(m)) + (t & m);
                    if (n >= static_cast<number_type>(m)) n -= m;
                } else {
                    if (!a) n = 0;
                    else if (n <= max_num / a.n) (n *= a.n) %= m;
                    else n = (unsigned __int128)n * a.n % m;
                }
            #endif
        }
        return *this;
    }
    constexpr StaticFp& operator/=(const StaticFp& a) noexcept {
        *this *= a.inv();
        return *this;
    }
    constexpr StaticFp operator+(const StaticFp& a) const noexcept {
        return StaticFp(*this) += a;
    }
    template<typename T> friend constexpr StaticFp operator+(const T& a, const StaticFp& b) noexcept {
        return b + a;
    }
    constexpr StaticFp operator-(const StaticFp& a) const noexcept {
        return StaticFp(*this) -= a;
    }
    template<typename T> friend constexpr StaticFp operator-(const T& a, const StaticFp& b) noexcept {
        return -(b - a);
    }
    constexpr StaticFp operator*(const StaticFp& a) const noexcept {
        return StaticFp(*this) *= a;
    }
    template<typename T> friend constexpr StaticFp operator*(const T& a, const StaticFp& b) noexcept {
        return b * a;
    }
    constexpr StaticFp operator/(const StaticFp& a) const noexcept {
        return StaticFp(*this) /= a;
    }
    template<typename T> friend constexpr StaticFp operator/(const T& a, const StaticFp& b) noexcept {
        return (b / a).inv();
    }
    constexpr StaticFp pow(const number_type& a) const noexcept {
        if (!a) return StaticFp(1);
        if (a < 0) return inv().pow(-a);
        StaticFp t = pow(a >> 1);
        t *= t;
        if (a & 1) t *= *this;
        return t;
    }
    constexpr StaticFp inv() const noexcept {
        StaticFp u, v;
        extended_euclidean<number_type, StaticFp>(n, m, u, v);
        return u;
    }
    constexpr bool operator==(const StaticFp& a) const noexcept {
        return n == a.n;
    }
    constexpr bool operator!=(const StaticFp& a) const noexcept {
        return n != a.n;
    }
    friend constexpr istream& operator>>(istream& s, StaticFp& a) {
        return s >> a.n;
    }
    friend constexpr ostream& operator<<(ostream& s, const StaticFp& a) {
        return s << a.n;
    }
};
using Fp9 = StaticFp<998244353>;
using Fp1 = StaticFp<1000000007>;

using mod_id_type = int_fast32_t;
template<mod_id_type id>
struct DynamicFp {
    using number_type = int_fast64_t;
    constexpr static number_type max_num = numeric_limits<number_type>::max();
    constexpr static mod_type DEFAULT_MOD = 998244353;
private:
    static mod_type m;
    static Barrett bt;
    static bool instances_created;
    number_type n;
    constexpr bool is_barrett_safe() {
        return m <= static_cast<mod_type>(Barrett::max_mod());
    }
    constexpr bool use_barrett() {
        return true && is_barrett_safe();
    }
    constexpr void fix_negative() noexcept {
        if (n < 0) n += m;
    }
public:
    constexpr DynamicFp(number_type nn = 0) noexcept : n(nn % m) {
        fix_negative();
        instances_created = true;
    }
    constexpr DynamicFp(number_type nn, mod_type mm) : n(nn % mm) {
        fix_negative();
        set_mod(mm);
        instances_created = true;
    }
    constexpr mod_type mod() noexcept {
        return m;
    }
    constexpr void set_mod(mod_type mm) {
        if (instances_created) throw runtime_error("mod cannot be reset after instance(s) are created");
        if (mm < 2) throw invalid_argument("mod must be two or greater");
        else if (mm > max_num) throw invalid_argument("mod must not overflow in number_type");
        bt = Barrett(m = mm);
    }
    template<typename T> constexpr operator T() const {
        return T(n);
    }
    constexpr DynamicFp operator+() const noexcept {
        return *this;
    }
    constexpr DynamicFp operator-() const noexcept {
        return n ? m - n : 0;
    }
    constexpr DynamicFp& operator++() noexcept {
        return *this += 1;
    }
    constexpr DynamicFp operator++(int) noexcept {
        DynamicFp t(*this);
        ++(*this);
        return t;
    }
    constexpr DynamicFp& operator--() noexcept {
        return *this -= 1;
    }
    constexpr DynamicFp operator--(int) noexcept {
        DynamicFp t(*this);
        --(*this);
        return t;
    }
    constexpr DynamicFp& operator+=(const DynamicFp& a) noexcept {
        if ((n += a.n) >= static_cast<number_type>(m)) n -= m;
        return *this;
    }
    constexpr DynamicFp& operator-=(const DynamicFp& a) noexcept {
        return *this += -a;
    }
    constexpr DynamicFp& operator*=(const DynamicFp& a) noexcept {
        if (use_barrett()) n = bt.mul(n, a.n);
        else {
            if (!a) n = 0;
            else if (n <= max_num / a.n) (n *= a.n) %= m;
            else {
                DynamicFp t(*this);
                (t += t) *= DynamicFp(a.n >> 1);
                if (a.n & 1) *this += t;
                else *this = t;
            }
        }
        return *this;
    }
    constexpr DynamicFp& operator/=(const DynamicFp& a) noexcept {
        *this *= a.inv();
        return *this;
    }
    constexpr DynamicFp operator+(const DynamicFp& a) const noexcept {
        return DynamicFp(*this) += a;
    }
    template<typename T> friend constexpr DynamicFp operator+(const T& a, const DynamicFp& b) noexcept {
        return b + a;
    }
    constexpr DynamicFp operator-(const DynamicFp& a) const noexcept {
        return DynamicFp(*this) -= a;
    }
    template<typename T> friend constexpr DynamicFp operator-(const T& a, const DynamicFp& b) noexcept {
        return -(b - a);
    }
    constexpr DynamicFp operator*(const DynamicFp& a) const noexcept {
        return DynamicFp(*this) *= a;
    }
    template<typename T> friend constexpr DynamicFp operator*(const T& a, const DynamicFp& b) noexcept {
        return b * a;
    }
    constexpr DynamicFp operator/(const DynamicFp& a) const noexcept {
        return DynamicFp(*this) /= a;
    }
    template<typename T> friend constexpr DynamicFp operator/(const T& a, const DynamicFp& b) noexcept {
        return (b / a).inv();
    }
    constexpr DynamicFp pow(const number_type& a) const noexcept {
        if (!a) return DynamicFp(1);
        if (a < 0) return inv().pow(-a);
        DynamicFp t = pow(a >> 1);
        t *= t;
        if (a & 1) t *= *this;
        return t;
    }
    constexpr DynamicFp inv() const noexcept {
        DynamicFp u(m), v(m);
        extended_euclidean<number_type, DynamicFp>(n, m, u, v);
        return u;
    }
    constexpr bool operator==(const DynamicFp& a) const noexcept {
        return n == a.n;
    }
    constexpr bool operator!=(const DynamicFp& a) const noexcept {
        return n != a.n;
    }
    friend istream& operator>>(istream& s, DynamicFp& a) {
        return s >> a.n;
    }
    friend ostream& operator<<(ostream& s, const DynamicFp& a) {
        return s << a.n;
    }
};
template<mod_id_type id>
mod_type DynamicFp<id>::m{DynamicFp<id>::DEFAULT_MOD};
template<mod_id_type id>
Barrett DynamicFp<id>::bt{DynamicFp<id>::DEFAULT_MOD};
template<mod_id_type id>
bool DynamicFp<id>::instances_created = false;
using Fp = DynamicFp<-1>;

template<class T>
struct BinomialCoefficient {
    using index_type = long long;
    vector<T> fact_, inv_, finv_;
    constexpr BinomialCoefficient() noexcept {}
    constexpr BinomialCoefficient(index_type n) noexcept : fact_(n, 1), inv_(n, 1), finv_(n, 1) {
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

using usec = chrono::microseconds;

usec tNow() {
    return chrono::duration_cast<usec>(chrono::system_clock::now().time_since_epoch());
}

int main() {
    constexpr int MOD = 998244353;
    constexpr long long MODL = 29996224275833;
    // constexpr int INF = (1 << 30);
    constexpr long long INFL = (1LL << 62);
    long long x, y;

    // cout << StaticFp<1>(10) << '\n'; // static assertion fails

    BinomialCoefficient< StaticFp<MOD> > bc(10);
    cout << bc.finv(9) << '\n'; // 712324701 (StaticFp<mod>(9)!.inv())
    cout << bc.finv(10) << '\n'; // undefined behaviour (accessing out of range using operator[])
    cout << '\n';

    cout << extended_euclidean<long long, long long>(111, 30, x, y) << '\n'; // 3 (= gcd(111, 30))
    cout << x << ", " << y << '\n'; // 3, -11 (111 * 3 + 30 * (-11) = 3 = gcd(111, 30))
    cout << '\n';

    StaticFp<MOD> d;
    cout << d << '\n'; // 0
    cout << '\n';

    long long t = StaticFp<MOD>(2);
    cout << (t - 3) << '\n'; // -1
    cout << '\n';

    StaticFp<MOD> a(2);
    cout << (++a) << '\n' << (a++) << '\n' << a << '\n' << (--a) << '\n' << (a--) << '\n' << a << '\n'; // 3, 3, 4, 3, 3, 2
    cout << '\n';

    cout << (a -= 3) << '\n' << (a += 3) << '\n' << (a /= 3) << '\n' << (a *= 3) << '\n'; // 998244352, 2, 665496236, 2
    cout << '\n';

    cout << (a + 1) << '\n' << a << '\n' << (1 + a) << '\n' << a << '\n' << '\n'; // 3, 2, 3, 2
    cout << '\n';

    cout << (a - 1) << '\n' << a << '\n' << (1 - a) << '\n' << a << '\n' << '\n'; // 1, 2, 998244352, 2
    cout << '\n';

    cout << +a << '\n' << a << '\n' << -a << '\n' << a << '\n'; // 2, 2, 998244351, 2
    cout << '\n';

    cout << a.pow(3) << '\n' << a.pow(100) << '\n' << a.pow(1000000) << '\n' << a << '\n'; // 8, 882499718, 421273117, 2
    cout << '\n';

    cout << a.inv() << '\n' << a << '\n'; // 499122177, 2
    cout << '\n';

    StaticFp<MODL> b(INFL);
    cout << b << '\n'; // 6505812270818
    cout << b * INFL << '\n'; // 6130151462911
    cout << (b *= INFL) << '\n'; // 6130151462911
    cout << '\n';

    cout << StaticFp<MODL>(2).pow(122) << '\n'; // 9031593934686
    cout << StaticFp<MODL>(2).pow(244) << '\n'; // 10412815166631
    cout << StaticFp<MODL>(2).pow(1000000) << '\n'; // 13282742706460
    cout << '\n';

    usec st;

    st = tNow();
    StaticFp<MODL> basel(1);
    for (int i = 0; i < 1000000000; ++i) basel *= 2;
    cout << basel << '\n'; // 26024802902219
    cout << basel.inv() << '\n'; // 2636315093014
    cerr << (tNow() - st).count() << '\n';
    cout << '\n';

    st = tNow();
    StaticFp<MOD> base(1);
    for (int i = 0; i < 1000000000; ++i) base *= 2;
    cout << base << '\n'; // 851104391
    cout << base.inv() << '\n'; // 394316601
    cerr << (tNow() - st).count() << '\n';
    cout << '\n';

    cout << '\n';

    DynamicFp<1> da(2, 998244353);
    cout << da.mod() << '\n'; // 998244353
    cout << '\n';

    cout << (++da) << '\n' << (da++) << '\n' << da << '\n' << (--da) << '\n' << (da--) << '\n' << da << '\n'; // 3, 3, 4, 3, 3, 2
    cout << '\n';

    cout << (da -= 3) << '\n' << (da += 3) << '\n' << (da /= 3) << '\n' << (da *= 3) << '\n'; // 998244352, 2, 665496236, 2
    cout << '\n';

    cout << (da + 1) << '\n' << da << '\n' << (1 + da) << '\n' << da << '\n' << '\n'; // 3, 2, 3, 2
    cout << '\n';

    cout << (da - 1) << '\n' << da << '\n' << (1 - da) << '\n' << da << '\n' << '\n'; // 1, 2, 998244352, 2
    cout << '\n';

    cout << +da << '\n' << da << '\n' << -da << '\n' << da << '\n'; // 2, 2, 998244351, 2
    cout << '\n';

    cout << da.pow(3) << '\n' << da.pow(100) << '\n' << da.pow(1000000) << '\n' << da << '\n'; // 8, 882499718, 421273117, 2
    cout << '\n';

    cout << da.inv() << '\n' << da << '\n'; // 499122177, 2
    cout << '\n';

    DynamicFp<2> db(INFL, MODL);
    cout << db << '\n'; // 6505812270818
    cout << db * INFL << '\n'; // 6130151462911
    cout << (db *= INFL) << '\n'; // 6130151462911
    cout << '\n';

    cout << DynamicFp<2>(2).pow(122) << '\n'; // 9031593934686
    cout << DynamicFp<2>(2).pow(244) << '\n'; // 10412815166631
    cout << DynamicFp<2>(2).pow(1000000) << '\n'; // 13282742706460
    cout << '\n';

    st = tNow();
    DynamicFp<1> dbase(1);
    for (int i = 0; i < 1000000000; ++i) dbase *= 2;
    cout << dbase << '\n'; // 851104391
    cout << dbase.inv() << '\n'; // 394316601
    cerr << (tNow() - st).count() << '\n'; // 27203 ms w/o Barrett, 8759 ms w/ Barrett
    cout << '\n';

    return 0;
}
