#include <iostream>
#include <limits>
#include <random>
#include <chrono>

using namespace std;

using mod_type = uint_fast64_t;

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
    static_assert(m >= 2, "mod must be greater than or equal to 2");
    using number_type = long long;
    constexpr static number_type max_num = numeric_limits<number_type>::max();
    static_assert(m <= max_num, "mod must not overflow in number_type");
private:
    number_type n;
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
        if ((n += a.n) >= static_cast<number_type>(m)) n -= m;
        return *this;
    }
    constexpr Fp& operator-=(const Fp& a) noexcept {
        return *this += -a;
    }
    constexpr Fp& operator*=(const Fp& a) noexcept {
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

template<class T>
struct is_duration : false_type {};
template<class Rep, class Period>
struct is_duration< chrono::duration<Rep, Period> > : true_type {};
template<typename duration_type>
duration_type current_t() {
    static_assert(is_duration<duration_type>::value);
    return chrono::duration_cast<duration_type>(chrono::high_resolution_clock::now().time_since_epoch());
}
template<typename duration_type>
struct RuntimeMeasurer {
    static_assert(is_duration<duration_type>::value, "RuntimeMeasurer must be used with a chrono::duration type");
private:
    duration_type st;
public:
    RuntimeMeasurer() : st(current_t<duration_type>()) {}
    duration_type start() {
        return st = current_t<duration_type>();
    }
    auto elapsed() const {
        return (current_t<duration_type>() - st).count();
    }
    friend ostream& operator<<(ostream& s, const RuntimeMeasurer& a) {
        return s << a.elapsed();
    }
};

struct RollingHash {
    using size_type = int;
    static constexpr long long m = (1LL << 61) - 1;
    random_device r;
    mt19937_64 mt;
    uniform_int_distribution<long long> ud;
    long long base;
    size_type n;
    vector< Fp<m> > hash_, exp;
private:
    uint_fast32_t xorshift(uint_fast32_t x) const {
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        return x;
    }
public:
    RollingHash(const string& s) : r(), mt(r()), ud(2, m - 2), base(ud(mt)), n(s.size()), hash_(n + 1), exp(n + 1, 1) {
        for (size_type i = 0; i < n; ++i) {
            hash_[i + 1] = hash_[i] * base + xorshift(s[i] + 1);
            exp[i + 1] = exp[i] * base;
        }
    }
    size_type size() const {
        return n;
    }
    // hash: return hash of s
    Fp<m> hash() const {
        return hash_.back();
    }
    // hash(l – 0-indexed, r – 0-indexed): return hash of s[l:r]
    Fp<m> hash(size_type l, size_type r) const {
        return hash_[r] - hash_[l] * exp[r - l];
    }
    // helper methods to check whether given substrings are the common prefix
    bool is_cp(size_type a, size_type b, size_type len) const {
        return hash(a, a + len) == hash(b, b + len);
    }
    bool is_cp(const RollingHash& t, size_type a, size_type b, size_type len) const {
        return hash(a, a + len) == t.hash(b, b + len);
    }
    // lcp_b(a – 0-indexed, b – 0-indexed): return lcp – longest common prefix – of s[a:] and s[b:] using binary search
    size_type lcp_b(size_type a, size_type b) const {
        size_type mi = 0, ma = n + 1 - max(a, b);
        while (ma - mi > 1) {
            size_type mid = (mi + ma) >> 1;
            (is_cp(a, b, mid) ? mi : ma) = mid;
        }
        return mi;
    }
    // lcp of s[a:] and t[b:] using binary search
    size_type lcp_b(const RollingHash& t, size_type a, size_type b) const {
        size_type mi = 0, ma = n + 1 - max(a, b);
        while (ma - mi > 1) {
            size_type mid = (mi + ma) >> 1;
            (is_cp(t, a, b, mid) ? mi : ma) = mid;
        }
        return mi;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // test with ABC141_E: Who Says a Pun?
    int n, ans1 = 0, ans2 = 0;
    string s;
    cin >> n >> s;
    RuntimeMeasurer<chrono::microseconds> rm;
    RollingHash rh(s);
    for (int i = 0; i < n; ++i) for (int j = i + ans1 + 1; j < n; ++j) ans1 = max(ans1, min(rh.lcp_b(i, j), j - i));
    for (int i = 0; i < n; ++i) for (int j = i + ans2 + 1; j < n; ++j) while (ans2 + 1 <= j - i && rh.is_cp(i, j, ans2 + 1)) ++ans2;
    cout << ans1 << '\n';
    cout << ans2 << '\n';
    cout << rm << '\n';
    return 0;
}
