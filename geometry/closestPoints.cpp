#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

using namespace std;

template<typename number_type, typename exponent_type>
constexpr auto pow_(number_type n, exponent_type a) {
    if constexpr (is_integral<exponent_type>::value) {
        if (a < 0) {
            number_type inv = 1 / n;
            if (inv == 0) return 0;
            else return pow_(inv, -a);
        }
        if (a == 0) return 1;
        number_type res = pow_(n, a >> 1);
        res *= res;
        if (a & 1) res *= n;
        return res;
    } else return std::pow(n, a);
}

template<typename coordinate_type>
struct Point {
    coordinate_type x, y;
    Point(coordinate_type xx = 0, coordinate_type yy = 0) : x(xx), y(yy) {}
    coordinate_type dist_from_origin_squared() const {
        return pow_(x, 2) + pow_(y, 2);
    }
    auto dist_from_origin() const {
        return sqrt(dist_from_origin_squared());
    }
    coordinate_type dist_squared(const Point& p) const {
        return (*this - p).dist_from_origin_squared();
    }
    auto dist(const Point& p) const {
        return sqrt(dist_squared(p));
    }
    Point& operator+=(const Point& a) {
        x += a.x, y += a.y;
        return *this;
    }
    Point operator+(const Point& a) const {
        return Point(*this) += a;
    }
    Point& operator-=(const Point& a) {
        x -= a.x, y -= a.y;
        return *this;
    }
    Point operator-(const Point& a) const {
        return Point(*this) -= a;
    }
    Point& operator*=(const coordinate_type& a) {
        x *= a, y *= a;
        return *this;
    }
    Point operator*(const coordinate_type& a) const {
        return Point(*this) *= a;
    }
    Point& operator/=(const coordinate_type& a) {
        x /= a, y /= a;
        return *this;
    }
    Point operator/(const coordinate_type& a) const {
        return Point(*this) /= a;
    }
    Point midpoint(const Point& a) const {
        return (*this + a) / 2;
    }
    friend istream& operator>>(istream& s, Point& a) {
        return s >> a.x >> a.y;
    }
    friend ostream& operator<<(ostream& s, const Point& a) {
        return s << a.x << ',' << a.y;
    }
};

template<typename coordinate_type>
struct ClosestPoints {
    using size_type = int;
    size_type n;
    vector< Point<coordinate_type> > v;
    ClosestPoints(size_type nn = 0) : n(nn), v(nn) {}
    ClosestPoints(initializer_list< Point<coordinate_type> > vv) : n(vv.size()), v(vv.begin(), vv.end()) {}
    ClosestPoints(vector< Point<coordinate_type> > vv) : n(vv.size()), v(vv) {}
    coordinate_type closest_squared_brute_force() const {
        vector<coordinate_type> t;
        t.reserve(n * (n - 1) / 2);
        for (size_type i = 0; i < n; ++i) for (size_type j = i + 1; j < n; ++j) t.push_back(v.at(i).dist_squared(v.at(j)));
        return *min_element(t.begin(), t.end());
    }
    friend coordinate_type closest_squared_util(const ClosestPoints& px, const ClosestPoints& py) {
        size_type n = px.n;
        if (n <= 3) return px.closest_squared_brute_force();
        size_type mid = n / 2;
        Point<coordinate_type> midpoint = px.v.at(mid);
        ClosestPoints pyl(mid), pyr(n - mid);
        size_type li = 0, ri = 0;
        for (size_type i = 0; i < n; ++i) {
            if (py.v.at(i).x <= midpoint.x && li < mid) pyl.v.at(li++) = py.v.at(i);
            else pyr.v.at(ri++) = py.v.at(i);
        }
        ClosestPoints pxl(mid), pxr(n - mid);
        for (size_type i = 0; i < mid; ++i) pxl.v.at(i) = px.v.at(i);
        for (size_type i = mid; i < n; ++i) pxr.v.at(i - mid) = px.v.at(i);
        return min(closest_squared_util(pxl, pyl), closest_squared_util(pxr, pyr));
    }
    coordinate_type closest_squared() const {
        ClosestPoints px(*this), py(*this);
        sort(px.v.begin(), px.v.end(), [](auto i, auto j) {
            return i.x < j.x;
        });
        sort(py.v.begin(), py.v.end(), [](auto i, auto j) {
            return i.y < j.y;
        });
        return closest_squared_util(px, py);
    }
    auto closest() const {
        return sqrt(closest_squared());
    }
};

int main() {
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> ud(-100, 100);
    ClosestPoints<int> p(vector< Point<int> >(20));
    for (auto&& i : p.v) {
        i.x = ud(mt), i.y = ud(mt);
        cout << i << '\n';
    }
    cout << "The smallest distance is " << p.closest() << '\n';
    return 0;
}
