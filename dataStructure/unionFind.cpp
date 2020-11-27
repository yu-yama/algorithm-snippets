#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

struct UnionFind {
    using size_type = int;
    using index_type = int;
    constexpr static index_type ROOT_PARENT = -1;
    static_assert(ROOT_PARENT < 0, "Default value of a UnionFind parent table must be negative");
    vector<index_type> parent, rank;
    UnionFind(size_type n = 0) : parent(n, ROOT_PARENT), rank(n, 0) {}
    index_type root(index_type x) {
        return parent.at(x) == ROOT_PARENT ? x : parent.at(x) = root(parent.at(x));
    }
    bool in_same(index_type x, index_type y) {
        return root(x) == root(y);
    }
    bool unite(index_type x, index_type y) {
        x = root(x), y = root(y);
        if (x == y) return false;
        if (rank.at(x) < rank.at(y)) swap(x, y);
        else if (rank.at(x) == rank.at(y)) ++rank.at(x);
        parent.at(y) = x;
        return true;
    }
};

struct UnionFindBySize {
    using size_type = int;
    using index_type = int;
    constexpr static index_type ROOT_PARENT = -1;
    static_assert(ROOT_PARENT < 0, "Default value of a UnionFind parent table must be negative");
    vector<index_type> parent;
    UnionFindBySize(size_type n = 0) : parent(n, ROOT_PARENT) {}
    index_type root(index_type x) {
        return parent.at(x) < 0 ? x : parent.at(x) = root(parent.at(x));
    }
    bool in_same(index_type x, index_type y) {
        return root(x) == root(y);
    }
    bool unite(index_type x, index_type y) {
        x = root(x), y = root(y);
        if (x == y) return false;
        if (parent.at(x) > parent.at(y)) swap(x, y);
        parent.at(x) += parent.at(y);
        parent.at(y) = x;
        return true;
    }
    index_type size(index_type x) {
        return -parent.at(root(x));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q, t, u, v;
    cin >> n >> q;
    UnionFind a(n + 1);
    for (int i = 0; i < q; ++i) {
        cin >> t >> u >> v;
        if (t) cout << a.in_same(u, v) << '\n';
        else a.unite(u, v);
    }
    return 0;
}
