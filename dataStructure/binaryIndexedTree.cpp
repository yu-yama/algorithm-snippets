#include <cstdio>
#include <vector>

using namespace std;
typedef long long ll;

// Binary Indexed Tree (Fenwick Tree)
template<typename T>
struct BIT {
    ll n;
    vector<T> d;
    BIT(ll n = 0) : n(n), d(n + 1) {}
    void add(ll i, T x = 1) {
        for (i++; i <= n; i += i & -i) d.at(i) += x;
    }
    T sum(ll i) {
        T x = 0;
        for (i++; i; i -= i & -i) x += d.at(i);
        return x;
    }
    T rangesum(ll l, ll r) {
        return sum(r) - sum(l - 1);
    }
};

int main() {
    ;
    return 0;
}
