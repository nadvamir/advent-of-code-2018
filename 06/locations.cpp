#include <algorithm>
#include <cassert>
#include <cmath>
#include <chrono>
#include <cstring>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>

struct Point {
    int i, j;
    bool infinite {false};
    bool operator<(const Point& o) const {
        return std::tie(i, j) < std::tie(o.i, o.j);
    }
    int distance(const Point& o) const {
        return std::abs(o.i - i) + std::abs(o.j - j);
    }
    bool operator==(const Point& o) const {
        return i == o.i && j == o.j;
    }
    bool operator!=(const Point& o) const {
        return !(o == *this);
    }
};

std::vector<Point> read_points(std::istream& is) {
    std::vector<Point> points;
    Point p;
    char c;
    while (is >> p.i >> c >> p.j) {
        points.push_back(p);
    }
    return points;
}

void mark_infinite(std::vector<Point>& points) {
    int minI = 1 << 10, minJ = 1 << 10, maxI = 0, maxJ = 0;
    for (auto& p : points) {
        if (p.i < minI) minI = p.i;
        if (p.j < minJ) minJ = p.j;
        if (p.i > maxI) maxI = p.i;
        if (p.j > maxJ) maxJ = p.j;
    }

    for (auto& p : points) {
        if (p.i <= minI + 1 || p.j <= minJ + 1 || p.i >= maxI - 1 || p.j >= maxJ - 1) {
            p.infinite = true;
        }
    }
}

void area(const std::vector<Point>& points, std::set<Point>& visited, Point p, Point& start) {
    if (visited.count(p)) return;
    if (start.infinite) return;
    if (p.i < 0 || p.j < 0 || p.i > 500 || p.j > 500) {
        start.infinite = true;
        return;
    }

    long dist = start.distance(p);
    for (const auto& o : points) {
        if (o.distance(p) <= dist && o != start) return;
    }

    visited.insert(p);

    area(points, visited, {p.i-1, p.j, false}, start);
    area(points, visited, {p.i+1, p.j, false}, start);
    area(points, visited, {p.i, p.j-1, false}, start);
    area(points, visited, {p.i, p.j+1, false}, start);
}

long largest_area(std::vector<Point>& points) {
    long result = 0;
    for (auto& point : points) {
        if (!point.infinite) {
            std::set<Point> visited {};
            area(points, visited, point, point);
            if (!point.infinite && visited.size() > result) {
                result = visited.size();
            }
        }
    }
    return result;
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    std::vector<Point> points {
        {1, 1, false},
        {1, 6, false},
        {8, 3, false},
        {3, 4, false},
        {5, 5, false},
        {8, 9, false}
    };

    mark_infinite(points);
    TEST_EQ(points[0].infinite, true);
    TEST_EQ(points[1].infinite, true);
    TEST_EQ(points[2].infinite, true);
    TEST_EQ(points[3].infinite, false);
    TEST_EQ(points[4].infinite, false);
    TEST_EQ(points[5].infinite, true);

    TEST_EQ(largest_area(points), 17);
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    auto points = read_points(std::cin);
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    mark_infinite(points);
    const auto answer = largest_area(points);

    high_resolution_clock::time_point t_end = high_resolution_clock::now();
    auto io_duration = duration_cast<microseconds>(t_io - t_start).count();
    auto all_duration = duration_cast<microseconds>(t_end - t_start).count();

    std::cout << answer << " in " << all_duration << "us (io: "
              << io_duration << "us)" << std::endl;
}

int main() {
    test();
    run();
}

