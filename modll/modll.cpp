#include <iostream>

using namespace std;
typedef long long ll;

const ll mod = 998244353;
struct modll {
    ll i;
    modll(int a = 0) : i(ll(a) % mod) {}
    modll(ll a = 0) : i(a % mod) {}
    modll operator+() {
        return *this;
    }
    modll operator-() {
        modll t(*this);
        if (t.i) t.i = mod - t.i;
        return t;
    }
    modll& operator++() {
        if (++i == mod) i = 0;
        return *this;
    }
    modll operator++(int) {
        modll t(*this);
        ++(*this);
        return t;
    }
    modll& operator--() {
        if (--i == -1) i = mod - 1;
        return *this;
    }
    modll operator--(int) {
        modll t(*this);
        --(*this);
        return t;
    }
    modll& operator+=(const modll a) {
        if ((i += a.i) >= mod) i -= mod;
        return *this;
    }
    modll& operator-=(const modll a) {
        if ((i += mod - a.i) >= mod) i -= mod;
        return *this;
    }
    modll& operator*=(const modll a) {
        (i *= a.i) %= mod;
        return *this;
    }
    modll operator+(const modll a) const {
        modll t(*this);
        return t += a;
    }
    modll operator+(const int a) const {
        return (*this) + modll(a);
    }
    modll operator+(const ll a) const {
        return (*this) + modll(a);
    }
    modll operator-(const modll a) const {
        modll t(*this);
        return t -= a;
    }
    modll operator-(const int a) const {
        return (*this) - modll(a);
    }
    modll operator-(const ll a) const {
        return (*this) - modll(a);
    }
    modll operator*(const modll a) const {
        modll t(*this);
        return t *= a;
    }
    modll operator*(const int a) const {
        return (*this) * modll(a);
    }
    modll operator*(const ll a) const {
        return (*this) * modll(a);
    }
    modll pow(ll a) const {
        if (a == 0) return 1;
        modll t = pow(a >> 1);
        t *= t;
        if (a & 1) t *= *this;
        return t;
    }

    operator ll() const {
        return i;
    }

    // only when mod is prime
    modll inv() const {
        return pow(mod - 2);
    }
    modll& operator/=(const modll a) {
        return (*this) *= a.inv();
    }
    modll operator/(const modll a) const {
        modll t(*this);
        return t /= a;
    }
    modll operator/(const int a) const {
        return (*this) / modll(a);
    }
    modll operator/(const ll a) const {
        return (*this) / modll(a);
    }
};

ostream& operator<<(ostream& s, const modll& a) {
    return s << a.i;
}
istream& operator>>(istream& s, modll& a) {
    return s >> a.i;
}

int main() {
    modll a(2);
    cout << (++a) << endl << (a++) << endl << a << endl << (--a) << endl << (a--) << endl << a << endl << (a -= 3) << endl << (a += 3) << endl << (a /= 3) << endl << (a *= 3) << endl << (a + 3) << endl << a << endl << +a << endl << a << endl << -a << endl << a << endl << a.pow(3) << endl << a << endl << a.pow(100) << endl << a << endl << a.inv() << endl << a << endl;
    return 0;
}
