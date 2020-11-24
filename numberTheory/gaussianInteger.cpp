#include <iostream>
#include <complex>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // gaussian integer can be implemented by complex<int>, provided in complex header
    complex<int> a(1, 2);
    cout << a << endl;
    return 0;
}
