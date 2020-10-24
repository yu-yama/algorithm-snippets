#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

struct UnionFind {
    vector<int> parent, rank;
    UnionFind(int n = 0) : parent(n, -1), rank(n, 0) {}
    int root(int x) {
        return (parent.at(x) == -1 ? x : parent.at(x) = root(parent.at(x)));
    }
    bool in_same(int x, int y) {
        return root(x) == root(y);
    }
    bool unite(int x, int y) {
        x = root(x), y = root(y);
        if (x == y) return false;
        if (rank.at(x) < rank.at(y)) swap(x, y);
        else if (rank.at(x) == rank.at(y)) ++rank.at(x);
        parent.at(y) = x;
        return true;
    }
};

struct UnionFindBySize {
    vector<int> parent;
    UnionFindBySize(int n = 0) : parent(n, -1) {}
    int root(int x) {
        return (parent.at(x) < 0 ? x : parent.at(x) = root(parent.at(x)));
    }
    bool in_same(int x, int y) {
        return root(x) == root(y);
    }
    bool unite(int x, int y) {
        x = root(x), y = root(y);
        if (x == y) return false;
        if (parent.at(x) > parent.at(y)) swap(x, y);
        parent.at(x) += parent.at(y);
        parent.at(y) = x;
        return true;
    }
    int size(int x) {
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
