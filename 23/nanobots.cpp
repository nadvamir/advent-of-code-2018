#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <chrono>
#include <cstring>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>

struct Coord {
    long x, y, z;
    long r;
    bool operator<(const Coord& o) const {
        return std::tie(r, x, y, z) < std::tie(o.r, o.x, o.y, o.z);
    }
    long distance(const Coord& o) const {
        return std::abs(o.x - x) + std::abs(o.y - y) + std::abs(o.z - z);
    }
    bool in_range(const Coord& o) const {
        return distance(o) <= r;
    }
};

std::vector<Coord> read_nanobots(std::istream& is) {
    std::vector<Coord> coords;
    char c;

    std::string line;
    while (getline(is, line)) {
        std::stringstream ss(line);
        Coord coord;
        ss.seekg(5);
        ss >> coord.x >> c >> coord.y >> c >> coord.z;
        ss.seekg(5, ss.cur);
        ss >> coord.r;
        coords.push_back(coord);
    }

    return coords;
}

Coord strongest_nanobot(const std::vector<Coord>& nanobots) {
    return *max_element(begin(nanobots), end(nanobots));
}

size_t num_in_range(const std::vector<Coord>& nanobots, const Coord& strongest) {
    return count_if(begin(nanobots), end(nanobots), [&](const Coord& bot) {
        return strongest.in_range(bot);
    });
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a)) << std::endl; assert((a) == (b)); }

void test() {
    std::string input = ""
        "pos=<0,0,0>, r=4\n"
        "pos=<1,0,0>, r=1\n"
        "pos=<4,0,0>, r=3\n"
        "pos=<0,2,0>, r=1\n"
        "pos=<0,5,0>, r=3\n"
        "pos=<0,0,3>, r=1\n"
        "pos=<1,1,1>, r=1\n"
        "pos=<1,1,2>, r=1\n"
        "pos=<1,3,1>, r=1\n";
    std::stringstream is(input);

    auto nanobots = read_nanobots(is);
    TEST_EQ(nanobots.size(), 9);

    auto strongest = strongest_nanobot(nanobots);
    TEST_EQ(strongest.r, 4);

    auto answer = num_in_range(nanobots, strongest);
    TEST_EQ(answer, 7);
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    auto nanobots = read_nanobots(std::cin);
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    auto strongest = strongest_nanobot(nanobots);
    auto answer = num_in_range(nanobots, strongest);

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

