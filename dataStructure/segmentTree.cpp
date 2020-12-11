#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

// find 2^k such that 2^(k - 1) < x <= 2^k
template <class T>
constexpr T bit_ceil(T x) {
    T t = x & -x;
    if (t == x) return x;
    return bit_ceil(x + t);
}

template<typename T>
struct SegmentTree {
    // the set must be form a monoid
    // – for any x, y, z in the set, op_(op_(x, y), z) = op_(x, op_(y, z)) [associativity], and
    // – e in the set exists such that, for any x in the set, op_(x, e) = op_(e, x) = x [existence of an ee element]
    using op_type = function<T(const T&, const T&)>;
    using size_type = int;
private:
    size_type n, capacity_;
    op_type op_;
    T e;
    vector<T> tr;
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
    constexpr SegmentTree() noexcept {}
    SegmentTree(size_type nn, const op_type& f, const T& ee, bool b = true): n(nn), capacity_(bit_ceil(n)), op_(f), e(ee), tr(capacity_ << 1, e), built(b) {}
    SegmentTree(const vector<T>& v, const op_type& f, const T& ee) : SegmentTree(v.size(), f, ee, false) {
        for (size_type i = 0; i < n; ++i) set(i, v.at(i));
    }
    size_type size() const noexcept {
        return n;
    }
    size_type capacity() const noexcept {
        return capacity_;
    }
    const op_type& op() const noexcept {
        return op_;
    }
    const T& identity() const noexcept {
        return e;
    }
    const vector<T>& tree() const noexcept {
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
    T get(size_type i, size_type j) {
        if (i < 0 || i >= n) throw out_of_range("beginning of interval is not in range");
        if (j <= i || j > n) throw out_of_range("end of interval is not in range");
        if (!built) build();
        T al = e, ar = e;
        for (size_type l = leaf_index(i), r = leaf_index(j); l < r; l >>= 1, r >>= 1) {
            if (l & 1) al = op_(al, tr[l++]);
            if (r & 1) ar = op_(tr[--r], ar);
        }
        return op_(al, ar);
    }
    T get(size_type i) {
        return tr[leaf_index(i)];
    }
    struct SegmentTreeElement {
        SegmentTree& t;
        size_type i;
        SegmentTreeElement(SegmentTree& tt, size_type ii) : t(tt), i(ii) {}
        const T& operator=(const T& x) {
            if (t.built) t.update(i, x);
            else t.set(i, x);
            return x;
        }
    };
    SegmentTreeElement at(size_type i) {
        check_size(i);
        return SegmentTreeElement(*this, i);
    }
    const T& at(size_type i) const {
        check_size();
        return tr[leaf_index(i)];
    }
    const T& operator[](size_type i) const {
        return tr[leaf_index(i)];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q, c, x, y;
    cin >> n >> q;
    // test using AOJ DSL_2_A
    SegmentTree<int> st(n, [](auto i, auto j) {
        return min(i, j);
    }, numeric_limits<int>::max());
    for (int i = 0; i < q; ++i) {
        cin >> c >> x >> y;
        switch (c) {
            case 0:
                st.at(x) = y;
                break;
            case 1:
                cout << st.get(x, y + 1) << '\n';
                break;
        }
    }
    // test using AOJ DSL_2_B
    cin >> n >> q;
    st = SegmentTree<int>(n + 1, [](auto i, auto j) {
        return i + j;
    }, 0);
    for (int i = 0; i < q; ++i) {
        cin >> c >> x >> y;
        switch (c) {
            case 0:
                st.at(x) = y;
                break;
            case 1:
                cout << st.get(x, y + 1) << '\n';
                break;
        }
    }
    return 0;
}
