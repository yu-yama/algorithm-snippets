#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <optional>

using namespace std;

// find 2^k such that 2^(k - 1) < x <= 2^k
template <class T>
constexpr T bit_ceil(T x) {
    T t = x & -x;
    if (t == x) return x;
    return bit_ceil(x + t);
}

template<typename T>
struct SegmentTreeOpt {
    // the set must be form a monoid
    // – for any x, y, z in the set, op_(op_(x, y), z) = op_(x, op_(y, z)) [associativity], and
    // – e in the set exists such that, for any x in the set, op_(x, e) = op_(e, x) = x [existence of an ee element]
    using opr_type = function<T(const T&, const T&)>;
    using size_type = int;
private:
    size_type n, capacity_;
    opr_type opr;
    optional<T> op_(const optional<T>& x, const optional<T>& y) {
        switch ((x.has_value() << 1) | y.has_value()) {
            case 0: return nullopt; break;
            case 1: return y; break;
            case 2: return x; break;
            default: return opr(x.value(), y.value()); break;
        }
    }
    vector< optional<T> > tr;
    bool built;
    void check_size(size_type i) const {
        if (i < 0 || i >= n) {
            ostringstream s;
            s << "SegmentTree::at out_of_range (index " << i << " given to a SegmentTree of length " << n << ")\n";
            throw out_of_range(s.str());
        }
    }
    size_type leaf_index(size_type i) const {
        return i + capacity_;
    }
    void update(size_type i) {
        tr[i] = op_(tr[i << 1], tr[(i << 1) | 1]);
    }
public:
    constexpr SegmentTreeOpt() noexcept {}
    SegmentTreeOpt(size_type nn, const opr_type& f, bool b = true): n(nn), capacity_(bit_ceil(n)), opr(f), tr(capacity_ << 1), built(b) {}
    SegmentTreeOpt(const vector<T>& v, const opr_type& f) : SegmentTreeOpt(v.size(), f, false) {
        for (size_type i = 0; i < n; ++i) set(i, v.at(i));
    }
    SegmentTreeOpt(const vector< optional<T> >& v, const opr_type& f) : SegmentTreeOpt(v.size(), f, false) {
        for (size_type i = 0; i < n; ++i) set(i, v.at(i));
    }
    size_type size() const noexcept {
        return n;
    }
    size_type capacity() const noexcept {
        return capacity_;
    }
    const opr_type& op() const noexcept {
        return opr;
    }
    const vector< optional<T> >& tree() const noexcept {
        return tr;
    }
    // indices are 0-indexed
    void set(size_type i, const T& v) {
        tr[leaf_index(i)] = v;
        built = false;
    }
    void build() {
        for (size_type k = capacity_ - 1; k > 0; --k) update(k);
        built = true;
    }
    void update(size_type i, const T& v) {
        size_type k = leaf_index(i);
        tr[k] = v;
        while (k >>= 1) update(k);
    }
    // get [i, j)
    optional<T> get(size_type i, size_type j) {
        if (i < 0 || i >= n) throw out_of_range("beginning of interval is not in range");
        if (j <= i || j > n) throw out_of_range("end of interval is not in range");
        if (!built) build();
        optional<T> al, ar;
        for (size_type l = leaf_index(i), r = leaf_index(j); l < r; l >>= 1, r >>= 1) {
            if (l & 1) al = op_(al, tr[l++]);
            if (r & 1) ar = op_(tr[--r], ar);
        }
        return op_(al, ar);
    }
    const optional<T>& get(size_type i) const {
        return tr[leaf_index(i)];
    }
    struct SegmentTreeOptElement {
    private:
        SegmentTreeOpt& t;
        size_type i;
    public:
        SegmentTreeOptElement(SegmentTreeOpt& tt, size_type ii) : t(tt), i(ii) {}
        const SegmentTreeOpt& segtree() const {
            return t;
        }
        size_type index() const {
            return i;
        }
        const T& operator=(const T& x) {
            if (t.built) t.update(i, x);
            else t.set(i, x);
            return x;
        }
        const T& operator+=(const T& x) {
            return *this = t.get(i) + x;
        }
        const T& operator-=(const T& x) {
            return *this = t.get(i) - x;
        }
        const T& operator*=(const T& x) {
            return *this = t.get(i) * x;
        }
        const T& operator/=(const T& x) {
            return *this = t.get(i) / x;
        }
        const T& operator%=(const T& x) {
            return *this = t.get(i) % x;
        }
    };
    SegmentTreeOptElement at(size_type i) {
        check_size(i);
        return SegmentTreeOptElement(*this, i);
    }
    const optional<T>& at(size_type i) const {
        check_size();
        return get(i);
    }
    const optional<T>& operator[](size_type i) const {
        return get(i);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q, c, x, y;
    cin >> n >> q;
    // test using AOJ DSL_2_A
    SegmentTreeOpt<int> st(n, [](auto i, auto j) {
        return min(i, j);
    });
    for (int i = 0; i < q; ++i) {
        cin >> c >> x >> y;
        switch (c) {
            case 0:
                st.at(x) = y;
                break;
            case 1:
                cout << st.get(x, y + 1).value_or(numeric_limits<int>::max()) << '\n';
                break;
        }
    }
    // test using AOJ DSL_2_B
    cin >> n >> q;
    st = SegmentTreeOpt<int>(n + 1, [](auto i, auto j) {
        return i + j;
    });
    for (int i = 0; i < q; ++i) {
        cin >> c >> x >> y;
        switch (c) {
            case 0:
                st.at(x) = y;
                break;
            case 1:
                cout << st.get(x, y + 1).value_or(0) << '\n';
                break;
        }
    }
    return 0;
}
