#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

#define double long double
using ll = long long;

const ll INF = 2'000'000'000;

const ll MOD = 1e9 + 7;

struct Point {
    double x = 0;
    double y = 0;
    Point() = default;
    Point(double xx, double yy) : x(xx), y(yy) {}

    Point& operator+=(Point point);
    Point& operator-=(Point point);
    Point& operator*=(double number);

    Point operator+(Point point) { return Point(*this) += point; }
    Point operator-(Point point) { return Point(*this) -= point; }
    Point operator*(double number) { return Point(*this) *= number; }
    double operator^(const Point& point) const { return x * point.y - y * point.x; }

    double get() { return x * y; }
};

Point& Point::operator+=(Point point) {
    x += point.x;
    y += point.y;
    return *this;
}

Point& Point::operator-=(Point point) {
    x -= point.x;
    y -= point.y;
    return *this;
}

Point& Point::operator*=(double number) {
    x *= number;
    y *= number;
    return *this;
}

std::istream& operator>>(std::istream& in, Point& p) {
    in >> p.x >> p.y;
    return in;
}

std::ostream& operator<<(std::ostream& out, Point& p) {
    out << p.x << " " << p.y;
    return out;
}

bool operator<(Point p1, Point p2) {
    return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
}

bool operator>(Point p1, Point p2) {
    return p2 < p1;
}

bool operator<=(Point p1, Point p2) {
    return !(p1 > p2);
}

bool operator>=(Point p1, Point p2) {
    return !(p2 < p1);
}

bool operator==(Point p1, Point p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

struct Line {
    double a;
    double b;
    double c;
    Line(const Point& p, const Point& q) : a(p.y - q.y), b(q.x - p.x), c(p.x * q.y - p.y * q.x) {}
};

bool are_on_line(const Point& p1, const Point& p2, const Point& p3) {
    Line l(p1, p2);
    return l.a * p3.x + l.b * p3.y + l.c == 0;
}

struct Segment {
    Point start;
    Point end;
    Segment() = default;
    Segment(const Point& p1, const Point& p2) : start(p1), end(p2) {}
    bool is_on(Point p) const;
};

bool Segment::is_on(const Point p) const {
    return are_on_line(start, end, p) && std::min(start.x, end.x) <= p.x && std::max(start.x, end.x) >= p.x &&
           std::min(start.y, end.y) <= p.y && std::max(start.y, end.y) >= p.y;
}

std::istream& operator>>(std::istream& in, Segment& s) {
    in >> s.start >> s.end;
    return in;
}

struct Vector {
    Point coords;
    explicit Vector(const Point& p) : coords(p) {}
    Vector(const Point& p1, const Point& p2) : coords(p2.x - p1.x, p2.y - p1.y) {}
    double len() const { return sqrtl(coords.x * coords.x + coords.y * coords.y); }
    Vector& operator+=(Vector vector);
    Vector operator+(Vector vector) { return Vector(*this) += vector; }
};

Vector& Vector::operator+=(Vector vector) {
    coords.x += vector.coords.x;
    coords.y += vector.coords.y;
    return *this;
}

double scalar_product(const Vector& v1, const Vector& v2) {
    return v1.coords.x * v2.coords.x + v1.coords.y * v2.coords.y;
}

double vector_product(Point a, Point b, Point c) {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
}

double point_line_distance(const Point& p, double a, double b, double c) {
    return std::abs(a * p.x + b * p.y + c) / sqrtl(a * a + b * b);
}

double area(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool are_intersect_coord(double a, double b, double c, double d) {
    if (a > b) {
        std::swap(a, b);
    }
    if (c > d) {
        std::swap(c, d);
    }
    return std::max(a, c) <= std::min(b, d);
}

bool are_intersect(const Segment& s1, const Segment& s2) {
    auto area1 = area(s1.start, s1.end, s2.start);
    auto area2 = area(s1.start, s1.end, s2.end);
    auto area3 = area(s2.start, s2.end, s1.start);
    auto area4 = area(s2.start, s2.end, s1.end);
    bool area_sign1 = (area1 > 0 && area2 > 0) || (area1 < 0 && area2 < 0);
    bool area_sign2 = (area3 < 0 && area4 < 0) || (area3 > 0 && area4 > 0);
    return !area_sign1 && !area_sign2 && are_intersect_coord(s1.start.x, s1.end.x, s2.start.x, s2.end.x) &&
           are_intersect_coord(s1.start.y, s1.end.y, s2.start.y, s2.end.y);
}

enum Belonging { INSIDE, OUTSIDE, BOUNDARY };

void reorder(vector<Point>& P) {
    size_t position = 0;
    for (size_t i = 1; i < P.size(); ++i) {
        if (P[i].y < P[position].y || (P[i].y == P[position].y && P[i].x < P[position].x)) {
            position = i;
        }
    }
    std::rotate(P.begin(), P.begin() + position, P.end());
}

class Polygon {
public:
    explicit Polygon(vector<Point> pp) : points(std::move(pp)) {}

    vector<Point> get_convex();
    Belonging is_in(Point p);
    vector<vector<bool>> segments();
    ll triangulations_count();

    Polygon& operator+=(Polygon polygon);

    Point& operator[](size_t position) { return points[position]; }

    size_t size() { return points.size(); }

private:
    vector<Point> points;
    size_t n = points.size();
};

vector<Point> Polygon::get_convex() {
    auto convex = points;
    sort(convex.begin(), convex.end());
    Point p1 = convex[0];
    Point p2 = convex.back();
    vector<Point> up;
    vector<Point> down;
    up.push_back(p1);
    down.push_back(p1);
    for (size_t i = 1; i < convex.size(); ++i) {
        if (i == convex.size() - 1 || vector_product(p1, convex[i], p2) < 0) {
            while (up.size() >= 2 && vector_product(up[up.size() - 2], up.back(), convex[i]) >= 0) {
                up.pop_back();
            }
            up.push_back(convex[i]);
        }
        if (i == convex.size() - 1 || vector_product(p1, convex[i], p2) > 0) {
            while (down.size() >= 2 && vector_product(down[down.size() - 2], down.back(), convex[i]) <= 0) {
                down.pop_back();
            }
            down.push_back(convex[i]);
        }
    }
    convex.clear();
    for (auto& point : up) {
        convex.push_back(point);
    }
    for (size_t i = down.size() - 2; i > 0; --i) {
        convex.push_back(down[i]);
    }
    return convex;
}

Belonging Polygon::is_in(Point p) {
    Point second(p.x + INF, p.y + 1);
    Segment segment(p, second);
    ll counter = 0;
    for (size_t i = 0; i < points.size() - 1; ++i) {
        Segment s(points[i], points[i + 1]);
        if (s.is_on(p)) {
            return Belonging::BOUNDARY;
        } else if (are_intersect(segment, {points[i], points[i + 1]})) {
            ++counter;
        }
    }
    Segment s(points[0], points.back());
    if (s.is_on(p)) {
        return Belonging::BOUNDARY;
    } else if (are_intersect(segment, {points[0], points.back()})) {
        ++counter;
    }
    if (counter & 1) {
        return Belonging::INSIDE;
    }
    return Belonging::OUTSIDE;
}

vector<vector<bool>> Polygon::segments() {
    vector<vector<bool>> res(n, vector<bool>(n, true));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (i == 0 && j == n - 1)
                continue;
            if (i == n - 1 && j == 0)
                continue;
            if (abs(int(i) - int(j)) <= 1)
                continue;
            if (i == j)
                continue;
            for (size_t k = 0; k < n + 1; ++k) {
                if (k % n == i || k % n == j || (k + 1) % n == i || (k + 1) % n == j) {
                    continue;
                }
                if (are_intersect({points[i], points[j]}, {points[k % n], points[(k + 1) % n]})) {
                    res[i][j] = false;
                    res[j][i] = false;
                }
            }
            if (is_in({(points[i].x + points[j].x) / 2, (points[i].y + points[j].y) / 2}) != Belonging::INSIDE) {
                res[i][j] = false;
                res[j][i] = false;
            }
        }
    }
    return res;
}

ll Polygon::triangulations_count() {
    vector<vector<bool>> segment = segments();
    vector<vector<ll>> dp(n, vector<ll>(n, 0));
    for (size_t i = 0; i < dp.size(); ++i) {
        dp[i % n][(i + 1) % n] = 1;
        dp[i % n][(i + 2) % n] = (segment[i % n][(i + 2) % n]) ? (1) : (0);
    }
    for (size_t k = 3; k < dp.size() + 1; ++k) {
        for (size_t i = 0; i <= n - k; ++i) {
            if (!segment[i % n][(i + k) % n]) {
                continue;
            }
            for (size_t j = i + 1; j < i + k; ++j) {
                dp[i % n][(i + k) % n] += (segment[i % n][j % n] && segment[j % n][(i + k) % n])
                                              ? (dp[i % n][j % n] * dp[j % n][(i + k) % n])
                                              : (0);
                dp[i % n][(i + k) % n] %= MOD;
            }
        }
    }
    return dp[0].back();
}

Polygon& Polygon::operator+=(Polygon polygon) {
    reorder(points);
    reorder(polygon.points);
    points.push_back(points[0]);
    points.push_back(points[1]);
    polygon.points.push_back(polygon.points[0]);
    polygon.points.push_back(polygon.points[1]);
    vector<Point> result;
    size_t i = 0, j = 0;
    while (i < points.size() - 2 || j < polygon.points.size() - 2) {
        result.push_back(points[i] + polygon.points[j]);
        ll cross = (points[i + 1] - points[i]) ^ (polygon.points[j + 1] - polygon.points[j]);
        if (cross >= 0) {
            ++i;
        }
        if (cross <= 0) {
            ++j;
        }
    }
    points = std::move(result);
    return *this;
}
