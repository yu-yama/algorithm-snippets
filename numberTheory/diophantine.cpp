#include <iostream>
#include <algorithm>
#include <optional>

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

template<typename T, typename U>
enable_if_t< is_integral<T>::value, bool > diophantine_any(T a, T b, T c, U& x0, U& y0, T& g) {
    g = extended_euclidean(abs(a), abs(b), x0, y0);
    if (c % g) return false;
    c /= g, x0 *= c, y0 *= c;
    if (a < 0) x0 = -x0;
    if (b < 0) y0 = -y0;
    return true;
}

template<typename T, typename U>
enable_if_t< is_integral<T>::value > diophantine_shift(T a, T b, T cnt, U& x, U& y) {
    x += cnt * b, y -= cnt * a;
}

template<typename T, typename U>
enable_if_t< is_integral<T>::value, int > diophantine_all(T a, T b, T c, U minx, U maxx, U miny, U maxy) {
    U x, y;
    T g;
    if (!diophantine_any(a, b, c, x, y, g)) return 0;
    a /= g, b /= g;

    int sign_a = a > 0 ? 1 : -1, sign_b = b > 0 ? 1 : -1;

    diophantine_shift(x, y, a, b, (minx - x) / b);
    if (x < minx) diophantine_shift(x, y, a, b, sign_b);
    if (x > maxx) return 0;
    int lx1 = x;
    diophantine_shift(x, y, a, b, (maxx - x) / b);
    if (x > maxx) diophantine_shift(x, y, a, b, -sign_b);
    int rx1 = x;

    diophantine_shift(x, y, a, b, -(miny - y) / a);
    if (y < miny) diophantine_shift(x, y, a, b, -sign_a);
    if (y > maxy) return 0;
    int lx2 = x;
    diophantine_shift(x, y, a, b, -(maxy - y) / a);
    if (y > maxy) diophantine_shift(x, y, a, b, sign_a);
    int rx2 = x;

    if (lx2 > rx2) swap(lx2, rx2);
    int lx = max(lx1, lx2);
    int rx = min(rx1, rx2);
    if (lx > rx) return 0;
    return (rx - lx) / abs(b) + 1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    using ll = long long;
    int x, y, z;
    cout << diophantine_any(2, 3, 7, x, y, z) << ' ' << x << ' ' << y << ' ' << z << '\n';
    ll a, b, c;
    cout << diophantine_any(2LL, 3LL, 7LL, a, b, c) << ' ' << a << ' ' << b << ' ' << c << '\n';
    cout << diophantine_any(2, 3, 7, a, b, z) << ' ' << a << ' ' << b << ' ' << z << '\n';
    return 0;
}
