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

long safest_area(std::vector<Point>& points, long target_dist) {
    long result = 0;

    Point p;
    for (p.i = 0; p.i < 500; ++p.i) {
        for (p.j = 0; p.j < 500; ++p.j) {
            long dist = 0;
            for (const auto& o : points) dist += o.distance(p);
            if (dist < target_dist) ++result;
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

    TEST_EQ(safest_area(points, 32), 16);
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    auto points = read_points(std::cin);
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    const auto answer = safest_area(points, 10000);

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

