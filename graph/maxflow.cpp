#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <cassert>
#include <queue>

using namespace std;

template<typename edge_type>
using Graph = vector< vector<edge_type> >;

template<typename capacity_type>
struct MaxFlowGraph {
    struct MaxFlowEdge {
        using size_type = int;
        size_type to, rev;
        capacity_type cap;
    };
    using size_type = typename MaxFlowEdge::size_type;
    Graph<MaxFlowEdge> g;
    vector< pair<size_type, int> > pos;
    MaxFlowGraph(size_type nn = 0) : g(nn) {}
    int add_edge(size_type from, size_type to, capacity_type cap) {
        assert(0 <= from && from < (size_type)g.size());
        assert(0 <= to && to < (size_type)g.size());
        assert(0 <= cap);
        int m = pos.size();
        pos.push_back({from, int(g[from].size())});
        int from_id = g[from].size();
        int to_id = g[to].size();
        if (from == to) ++to_id;
        g[from].push_back({to, to_id, cap});
        g[to].push_back({from, from_id, 0});
        return m;
    }
    struct FlowEdge {
        size_type from, to;
        capacity_type cap, flow;
    };
    FlowEdge get_edge(int i) {
        int m = pos.size();
        assert(0 <= i && i < m);
        auto e = g[pos[i].first][pos[i].second], re = g[e.to][e.rev];
        return {pos[i].first, e.to, e.cap + re.cap, re.cap};
    }
    vector<FlowEdge> edges() {
        int m = pos.size();
        vector<FlowEdge> res(m);
        for (int i = 0; i < m; ++i) res[i] = get_edge(i);
        return res;
    }
    void change_edge(int i, capacity_type new_cap, capacity_type new_flow) {
        int m = pos.size();
        assert(0 <= i && i < m);
        assert(0 <= new_flow && new_flow <= new_cap);
        auto& e = g[pos[i].first][pos[i].second];
        auto& re = g[e.to][e.rev];
        e.cap = new_cap - new_flow;
        re.cap = new_flow;
    }
    capacity_type flow(size_type from, size_type to, capacity_type flow_limit = numeric_limits<capacity_type>::max()) {
        using level_type = int;
        constexpr level_type LEVEL_DEFAULT = -1;
        static_assert(LEVEL_DEFAULT < 0);
        assert(0 <= from && from < (size_type)g.size());
        assert(0 <= to && to < (size_type)g.size());
        assert(from != to);
        vector<level_type> level(g.size());
        vector<int> iter(g.size());
        auto bfs = [&]() {
            fill(level.begin(), level.end(), LEVEL_DEFAULT);
            level[from] = 0;
            queue<size_type> q;
            q.push(from);
            while (!q.empty()) {
                size_type v = q.front();
                q.pop();
                for (const auto& e : g[v]) {
                    if (e.cap == 0 || level[e.to] >= 0) continue;
                    level[e.to] = level[v] + 1;
                    if (e.to == to) return;
                    q.push(e.to);
                }
            }
        };
        auto dfs = [&](auto self, int v, capacity_type up) {
            if (v == from) return up;
            capacity_type res = 0;
            int level_v = level[v];
            for (int& i = iter[v]; i < int(g[v].size()); i++) {
                MaxFlowEdge& e = g[v][i];
                if (level_v <= level[e.to] || g[e.to][e.rev].cap == 0) continue;
                capacity_type d = self(self, e.to, min(up - res, g[e.to][e.rev].cap));
                if (d <= 0) continue;
                g[v][i].cap += d;
                g[e.to][e.rev].cap -= d;
                res += d;
                if (res == up) return res;
            }
            level[v] = g.size();
            return res;
        };
        capacity_type flow = 0;
        while (flow < flow_limit) {
            bfs();
            if (level[to] == LEVEL_DEFAULT) break;
            fill(iter.begin(), iter.end(), 0);
            capacity_type f = dfs(dfs, to, flow_limit - flow);
            if (!f) break;
            flow += f;
        }
        return flow;
    }
    vector<bool> min_cut(int s) {
        vector<bool> visited(g.size());
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int p = q.front();
            q.pop();
            visited[p] = true;
            for (auto e : g[p]) {
                if (e.cap && !visited[e.to]) {
                    visited[e.to] = true;
                    q.push(e.to);
                }
            }
        }
        return visited;
    }
};


struct MaxBipartiteMatchingGraph : MaxFlowGraph<int> {
private:
    size_type n;
    map<int, int> source_side, sink_side;
    size_type source() const noexcept {
        return n;
    }
    size_type sink() const noexcept {
        return n + 1;
    }
public:
    MaxBipartiteMatchingGraph(size_type nn = 0) : MaxFlowGraph<int>(nn + 2), n(nn) {}
    size_type size() const noexcept {
        return n;
    }
    void add_edge(size_type from, size_type to) {
        assert(0 <= from && from < n);
        assert(0 <= to && to < n);
        if (source_side.count(to) || sink_side.count(from)) throw invalid_argument("graph must be bipartite");
        source_side[from], sink_side[to];
        MaxFlowGraph<int>::add_edge(from, to, 1);
    }
    int flow(size_type from, size_type to) {
        return flow(from, to, numeric_limits<int>::max());
    }
    int flow(size_type from, size_type to, int flow_limit) {
        for (auto&& i : source_side) if (i.second == 0) MaxFlowGraph<int>::add_edge(source(), i.first, 1), i.second = 1;
        for (auto&& i : sink_side) if (i.second == 0) MaxFlowGraph<int>::add_edge(i.first, sink(), 1), i.second = 1;
        return MaxFlowGraph<int>::flow(from, to, flow_limit);
    }
    int max_matching() {
        return flow(source(), sink());
    }
    vector<FlowEdge> matches() {
        vector<FlowEdge> res;
        for (const auto& i : MaxFlowGraph<int>::edges()) if (i.from != source() && i.to != sink() && i.flow != 0) res.push_back(i);
        return res;
    }
};

constexpr int twod_to_oned(int i, int j, int w) noexcept {
    return (i * w) + j;
}

constexpr pair<int, int> oned_to_twod(int i, int w) noexcept {
    return {i / w, i % w};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // test with Practice2_D: Maxflow
    int n, m;
    cin >> n >> m;
    vector< vector<char> > s(n + 2, vector<char>(m + 2));
    for (int i = 1; i <= n; ++i) for (int j = 1; j <= m; ++j) cin >> s.at(i).at(j);
    MaxBipartiteMatchingGraph g((n + 2) * (m + 2));
    vector< pair<int, int> > m4 = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    for (int i = 1; i <= n; ++i) for (int j = 1; j <= m; ++j) if (s.at(i).at(j) == '.' && (i + j) % 2) {
        for (const auto& k : m4) if (s.at(i + k.first).at(j + k.second) == '.') g.add_edge(twod_to_oned(i, j, m + 2), twod_to_oned(i + k.first, j + k.second, m + 2));
    }
    cout << g.max_matching() << '\n';
    for (const auto& i : g.matches()) {
        auto p = oned_to_twod(min(i.from, i.to), m + 2), q = oned_to_twod(max(i.from, i.to), m + 2);
        if (p.first == q.first) s.at(p.first).at(p.second) = '>', s.at(q.first).at(q.second) = '<';
        else s.at(p.first).at(p.second) = 'v', s.at(q.first).at(q.second) = '^';
    }
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) cout << s.at(i).at(j);
        cout << '\n';
    }
    cout << '\n';

    // test with sample graph
    MaxFlowGraph<int> h(6);
    h.add_edge(0, 1, 16);
    h.add_edge(0, 2, 13);
    h.add_edge(1, 2, 10);
    h.add_edge(1, 3, 12);
    h.add_edge(2, 1, 4);
    h.add_edge(2, 4, 14);
    h.add_edge(3, 2, 9);
    h.add_edge(3, 5, 20);
    h.add_edge(4, 3, 7);
    h.add_edge(4, 5, 4);
    cout << h.flow(0, 5) << '\n'; // 23
    for (const auto& i : h.edges()) cout << i.from << " -> " << i.to << ": " << i.flow << " / " << i.cap << '\n';
    // 0 -> 1: 12 / 16
    // 0 -> 2: 11 / 13
    // 1 -> 2: 0 / 10
    // 1 -> 3: 12 / 12
    // 2 -> 1: 0 / 4
    // 2 -> 4: 11 / 14
    // 3 -> 2: 0 / 9
    // 3 -> 5: 19 / 20
    // 4 -> 3: 7 / 7
    // 4 -> 5: 4 / 4
    cout << '\n';

    return 0;
}
