#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

template<typename number_type>
struct BIT {
    using index_type = long long;
    index_type n;
    vector<number_type> d;
    // constructor: create a binary indexed tree with n elements (0-indexed)
    BIT(index_type nn = 0) : n(nn), d(nn + 1) {}
    // add(i – 0-indexed, x – number to add): add x to the i-th element
    void add(index_type i, const number_type& x) {
        for (++i; i <= n; i += i & -i) d.at(i) += x;
    }
    // sum(i – 0-indexed): return sum of k-th elements where k ∈ [0, i)
    number_type sum(index_type i) const {
        number_type x = 0;
        for (; i > 0; i -= i & -i) x += d.at(i);
        return x;
    }
    // sum(i – 0-indexed, j – 0-indexed): return sum of k-th elements where k ∈ [i, j)
    number_type sum(index_type i, index_type j) const {
        return sum(j) - sum(i);
    }
    struct BITElement {
        using index_type = typename BIT<number_type>::index_type;
        BIT<number_type>& b;
        index_type i;
        BITElement(BIT<number_type>& bb, index_type ii) : b(bb), i(ii) {}
        BITElement& operator++() noexcept {
            return *this += 1;
        }
        BITElement& operator--() noexcept {
            return *this -= 1;
        }
        BITElement& operator+=(number_type x) {
            b.add(i, x);
            return *this;
        }
        BITElement& operator-=(number_type x) {
            return *this += -x;
        }
    };
    constexpr BITElement at(index_type i) {
        if (i < 0 || i >= n) {
            ostringstream e;
            e << "BIT::at out_of_range (index " << i << " given to a BIT of length " << n << ")\n";
            throw out_of_range(e.str());
        }
        return BITElement(*this, i);
    }
    constexpr number_type size() const noexcept {
        return n;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    using ll = long long;
    ll n, q, a, t, u, v;
    cin >> n >> q;
    BIT<ll> b(n);
    for (ll i = 0; i < n; ++i) cin >> a, b.add(i, a);
    for (ll i = 0; i < q; ++i) {
        cin >> t >> u >> v;
        if (t) cout << b.sum(u, v) << '\n';
        else b.at(u) += v;
    }
    return 0;
}
