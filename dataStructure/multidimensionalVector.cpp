#include <iostream>
#include <vector>

using namespace std;

template<typename T, typename D>
auto md_vector(T v, D dh) {
    return vector<T>(dh, v);
}
template<typename T, typename DHead, typename... DTail>
auto md_vector(T v, DHead dh, DTail... dt) {
    return md_vector(md_vector(v, forward<DTail>(dt)...), dh);
}
template<typename T, typename... D>
auto md_vector_d(D... d) {
    return md_vector(T{}, forward<D>(d)...);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    auto a = md_vector<long long>(2, 5, 3); // 5x3 2d vector of long long int with dimensions 5x3, each element initialized to 2
    for (const auto& i : a) {
        for (const auto& j : i) cout << j << ' ';
        cout << '\n';
    }
    auto b = md_vector(2, 10); // 1d vector of int (deduced from 2) with a dimension of 10, each element initialized to 2
    for (const auto& i : b) cout << i << ' ';
    cout << '\n';
    auto c = md_vector_d<int>(10); // 1d vector of int with a dimension of 10, each element initialized by the default value (0 for int)
    for (const auto& i : c) cout << i << ' ';
    cout << '\n';
    return 0;
}
