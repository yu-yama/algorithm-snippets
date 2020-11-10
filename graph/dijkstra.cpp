#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <limits>
#include <cassert>

using namespace std;

template<typename edge_type>
using Graph = vector< vector<edge_type> >;

template<typename cost_type>
struct Dijkstra {
    static constexpr cost_type INF = numeric_limits<cost_type>::max() >> 4;
    struct DijkstraEdge {
        using size_type = int;
        size_type to;
        cost_type cost;
    };
    using size_type = typename DijkstraEdge::size_type;
    Graph<DijkstraEdge> g;
    Dijkstra(size_type nn) : g(nn) {}
    void add_edge(size_type f, size_type t, cost_type c) {
        assert(0 <= f && f < (size_type)g.size());
        assert(0 <= t && t < (size_type)g.size());
        assert(0 <= c);
        g[f].push_back({t, c});
    }
    void add_undirected_edge(size_type f, size_type t, cost_type c) {
        add_edge(f, t, c);
        add_edge(t, f, c);
    }
    vector<cost_type> min_cost_from(size_type s) const {
        assert(0 <= s && s < (size_type)g.size());
        vector<cost_type> res(g.size(), INF);
        res[s] = 0;
        using cost_pair = pair<cost_type, size_type>;
        priority_queue<cost_pair, vector<cost_pair>, greater<cost_pair> > q;
        q.push({0, s});
        while (!q.empty()) {
            auto [c, v] = q.top();
            q.pop();
            if (res[v] < c) continue;
            for (const auto& i : g[v]) if (res[i.to] > res[v] + i.cost) {
                res[i.to] = res[v] + i.cost;
                q.push({res[i.to], i.to});
            }
        }
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // test with ABC035_D: Treasure Hunting
    using ll = long long;
    ll n, m, t, a, b, c, ans = 0;
    cin >> n >> m >> t;
    vector<ll> v(n + 1);
    for (int i = 1; i <= n; ++i) cin >> v.at(i);
    Dijkstra<ll> ef(n + 1), eb(n + 1);
    for (int i = 0; i < m; ++i) cin >> a >> b >> c, ef.add_edge(a, b, c), eb.add_edge(b, a, c);
    auto sf = ef.min_cost_from(1), sb = eb.min_cost_from(1);
    for (int i = 1; i <= n; ++i) if (sf.at(i) + sb.at(i) <= t) ans = max(ans, v.at(i) * (t - (sf.at(i) + sb.at(i))));
    cout << ans << '\n';
    return 0;
}
