// extend vector functions to string, map, pair
// define map sorting by the value, not the key

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <string>
#include <vector>

using namespace std;

using ll = long long;
using st = string;
using pi = pair<int, int>;
using pl = pair<ll, ll>;
using vc = vector<char>;
using vi = vector<int>;
using vpi = vector<pi>;
using vl = vector<ll>;
using vpl = vector<pl>;
using vst = vector<st>;
using vvc = vector<vc>;
using vvi = vector<vi>;
using vvpi = vector<vpi>;
using vvl = vector<vl>;
using vvpl = vector<vpl>;
using vvst = vector<vst>;
using vvvc = vector<vvc>;
using vvvi = vector<vvi>;
using vvvpi = vector<vvpi>;
using vvvl = vector<vvl>;
using vvvpl = vector<vvpl>;
using vvvst = vector<vvst>;
using mii = map<int, int>;
using mci = map<char, int>;
using msi = map<string, int>;
using msc = map<string, char>;

constexpr int INF = 1 << 30;
constexpr long long INFL = 1LL << 62;
constexpr int MOD1 = 1000000007;
constexpr int MOD9 = 998244353;
#define FST first
#define SND second
#define REP(i, l, r) for (ll i = l; i < static_cast<decltype(i)>(r); ++i)
#define REPZ(i, n) REP(i, 0, n)
#define REPE(i, l, r) for (ll i = l; i <= static_cast<decltype(i)>(r); ++i)
#define REPEZ(i, n) REPE(i, 0, n)
#define REPR(i, l, r) for (ll i = r - 1; i >= static_cast<decltype(i)>(l); --i)
#define REPRZ(i, n) REPR(i, 0, n)
#define REPRE(i, l, r) for (ll i = r; i >= static_cast<decltype(i)>(l); --i)
#define REPREZ(i, n) REPRE(i, 0, n)
#define ALL(v) (v).begin(), (v).end()
#define SRT(v) sort(ALL(v))
#define UNQ(v) sort(ALL(v)); v.erase(unique(ALL(v)), v.end())
#define YES(condition) cout << (condition ? "YES" : "NO") << endl
#define Yes(condition) cout << (condition ? "Yes" : "No") << endl
#define POSS(condition) cout << (condition ? "POSSIBLE" : "IMPOSSIBLE") << endl
#define Poss(condition) cout << (condition ? "Possible" : "Impossible") << endl

vpi m4 = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
vpi m8 = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};
st ___sin = "";
ll ___spos = 0;

template<typename T> T& getval(istream& s = cin, T& x) {
    return s >> x;
}
template<> int& getval(istream& s = cin, T& x) {
    if (___sin.empty()) getline(s, ___sin);
    bool neg = false, sp = true;
    int t = 0;
    for (auto i = ___sin.begin() + ___spos; i != ___sin.end(); ++i) switch (*i) {
        case ' ': if (sp) {
            if (i = ___sin.end() - 1) getline(s, ___sin);
            else continue;
        } else {
            return (x = (neg ? -t : t));
        } break;
        case '-': neg = true; break;
        default: sp = false, t = t * 10 + (*i - '0');
    }
}

template<typename T1, typename T2> istream& operator>>(istream& s, pair<T1,T2>& x) {
    return s >> x.FST >> x.SND;
}
template<typename T1, typename T2> ostream& operator<<(ostream& s, pair<T1,T2>& x) {
    return s << x.FST << " " << x.SND;
}
template<typename T> istream& operator>>(istream& s, vector<T>& v) {
    REPZ(i, v.size()) s >> v.at(i);
    return s;
}
template<typename T> ostream& operator<<(ostream& s, vector<T>& v) {
    REPZ(i, v.size()) s << v.at(i) << endl;
    return s;
}

template<typename T> ll count(vector<T>& v, const T n) {
    return count(v.begin(), v.end(), n);
}
template<typename T> ll count(vector< vector<T> >& v, const T n) {
    return accumulate(v.begin(), v.end(), 0LL, [n](ll init, vector<T> i) { return init + count(i, n); });
}

template<typename T> ll accumulate(vector<T>& v) {
    return accumulate(v.begin(), v.end(), 0LL);
}
template<typename T> ll accumulate(vector< vector<T> >& v) {
    return accumulate(v.begin(), v.end(), 0LL, [](ll init, vector<T> i) { return init + accumulate(i); });
}

template<typename T> ll uniq(vector<T>& v) {
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
}
template<typename T> ll uniq_(const vector<T> v) {
    vector<T> u(v);
    uniq(u);
    return u;
}
template<typename T> ll kind(const vector<T> v) {
    return uniq_(v).size();
}

ll fact(ll a) {
    if (a < 0) return 0;
    if (a < 2) return 1;
    REPR(i, 2, a) a *= i;
    return a;
}

vl dijkstra(ll s, vvpl e) {
    ll n = e.size();
    vl minD(n, LINF);
    minD.at(s) = 0;
    priority_queue< pl, vpl, greater<pl> > q;
    q.push(pl(0, s));
    while (!q.empty()) {
        pl now = q.top();
        q.pop();
        ll c = now.FST, v = now.SND;
        if (minD.at(v) < c) continue;
        REPZ(i, e.at(v).size()) {
            pl p = e.at(v).at(i);
            if (minD.at(p.FST) > minD.at(v) + p.SND) {
                minD.at(p.FST) = minD.at(v) + p.SND;
                q.push(pl(minD.at(p.FST), p.FST));
            }
        }
    }
    return minD;
}

vl dijkstra(ll s, vvvl e, ll t) {
    vl minD(e.size(), LINF);
    minD.at(s) = 0;
    priority_queue< pl, vpl, greater<pl> > q;
    q.push(pl(0, s));
    while (!q.empty()) {
        pl now = q.top();
        q.pop();
        ll c = now.FST, v = now.SND;
        if (minD.at(v) < c) continue;
        REPZ(i, e.at(v).size()) {
            pl p(e.at(v).at(i).at(0), e.at(v).at(i).at(t));
            if (minD.at(p.FST) > minD.at(v) + p.SND) {
                minD.at(p.FST) = minD.at(v) + p.SND;
                q.push(pl(minD.at(p.FST), p.FST));
            }
        }
    }
    return minD;
}

ll addMod(ll a, ll b) { return (a + b) % MOD; }
ll subMod(ll a, ll b) { return (a + MOD - b) % MOD; }
ll mulMod(ll a, ll b) { return a * b % MOD; }
ll powMod(ll n, ll r) {
    if (r == 0) return 1;
    if (r % 2) return mulMod(n, powMod(n, r - 1));
    ll temp = powMod(n, r / 2);
    return mulMod(temp, temp);
}
ll divMod(ll a, ll b) { return mulMod(a, powMod(b, MOD - 2)); }
ll combMod(ll n, ll r) {
    if (r > n - r) return combMod(n, n - r);
    ll deno = 1, nume = 1;
    for (ll i = 0; i < r; ++i) {
        nume = mulMod(nume, n - i);
        deno = mulMod(deno, i + 1);
    }
    return divMod(nume, deno);
}

int main() {
    ios::sync_with_stdio(false);
    /* code */
    return 0;
}
