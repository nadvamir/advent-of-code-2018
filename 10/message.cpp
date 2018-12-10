#include <algorithm>
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

struct Point {
    int x, y;

    Point operator+(Point o) const {
        return Point{x + o.x, y + o.y};
    }

    void paint(std::vector<std::string>& field, int minX, int minY) const {
        field[y - minY][x - minX] = '#';
    }
};

struct Light {
    Point position;
    Point velocity;

    static Light make(const std::string& line) {
        std::stringstream ss(line);
        Light l;
        char c;

        ss.seekg(10);
        ss >> l.position.x >> c >> l.position.y;
        ss.seekg(12, ss.cur);
        ss >> l.velocity.x >> c >> l.velocity.y;

        return l;
    }
};

std::vector<Light> read_input(std::istream& is) {
    std::vector<Light> lights;
    std::string line;
    while (std::getline(is, line)) {
        lights.emplace_back(Light::make(line));
    }
    return lights;
}

bool aligned(const std::vector<Light>& lights) {
    std::unordered_map<int, int> repeat_x;
    std::unordered_map<int, int> repeat_y;
    for (const auto& l : lights) {
        ++repeat_x[l.position.x];
        ++repeat_y[l.position.y];
    }

    int num_repeating_x = 0;
    for (const auto& x : repeat_x) {
        if (x.second > 6) ++num_repeating_x;
    }

    int num_repeating_y = 0;
    for (const auto& y : repeat_y) {
        if (y.second > 2) ++num_repeating_y;
    }

    return num_repeating_x > 2 && num_repeating_y > 6;
}

void iterate(std::vector<Light>& lights) {
    for (auto& l : lights) {
        l.position = l.position + l.velocity;
    }
}

bool print(const std::vector<Light>& lights) {
    int minX = 1 << 10, minY = 1 << 10;
    int maxX = 0, maxY = 0;
    for (const auto& l : lights) {
        minX = std::min(minX, l.position.x);
        minY = std::min(minY, l.position.y);
        maxX = std::max(maxX, l.position.x);
        maxY = std::max(maxY, l.position.y);
    }
    
    const int w = maxX - minX + 1;
    const int h = maxY - minY + 1;

    if (h > 10) return false;

    std::vector<std::string> sky;
    for (int i = 0; i < h; ++i) sky.emplace_back(std::string(w, '.'));

    for (const auto& l : lights) {
        l.position.paint(sky, minX, minY);
    }

    for (const auto& l: sky) {
        std::cout << l << std::endl;
    }
    return true;
}

int simulate(std::vector<Light>& lights, int n_alignments) {
    int iterations = 0;
    bool not_aligned = false;
    while (n_alignments--) {
        for (; !aligned(lights) || not_aligned; ++iterations) {
            not_aligned = false;
            iterate(lights);
        }
        if (!print(lights)) {
            ++n_alignments;
            not_aligned = true;
        }
    }
    return iterations;
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    std::string input = ""
        "position=< 9,  1> velocity=< 0,  2>\n"
        "position=< 7,  0> velocity=<-1,  0>\n"
        "position=< 3, -2> velocity=<-1,  1>\n"
        "position=< 6, 10> velocity=<-2, -1>\n"
        "position=< 2, -4> velocity=< 2,  2>\n"
        "position=<-6, 10> velocity=< 2, -2>\n"
        "position=< 1,  8> velocity=< 1, -1>\n"
        "position=< 1,  7> velocity=< 1,  0>\n"
        "position=<-3, 11> velocity=< 1, -2>\n"
        "position=< 7,  6> velocity=<-1, -1>\n"
        "position=<-2,  3> velocity=< 1,  0>\n"
        "position=<-4,  3> velocity=< 2,  0>\n"
        "position=<10, -3> velocity=<-1,  1>\n"
        "position=< 5, 11> velocity=< 1, -2>\n"
        "position=< 4,  7> velocity=< 0, -1>\n"
        "position=< 8, -2> velocity=< 0,  1>\n"
        "position=<15,  0> velocity=<-2,  0>\n"
        "position=< 1,  6> velocity=< 1,  0>\n"
        "position=< 8,  9> velocity=< 0, -1>\n"
        "position=< 3,  3> velocity=<-1,  1>\n"
        "position=< 0,  5> velocity=< 0, -1>\n"
        "position=<-2,  2> velocity=< 2,  0>\n"
        "position=< 5, -2> velocity=< 1,  2>\n"
        "position=< 1,  4> velocity=< 2,  1>\n"
        "position=<-2,  7> velocity=< 2, -2>\n"
        "position=< 3,  6> velocity=<-1, -1>\n"
        "position=< 5,  0> velocity=< 1,  0>\n"
        "position=<-6,  0> velocity=< 2,  0>\n"
        "position=< 5,  9> velocity=< 1, -2>\n"
        "position=<14,  7> velocity=<-2,  0>\n"
        "position=<-3,  6> velocity=< 2, -1>\n";
    std::stringstream ss(input);

    auto points = read_input(ss);

    TEST_EQ(points[5].position.x, -6);
    TEST_EQ(points[5].position.y, 10);
    TEST_EQ(points[3].velocity.x, -2);
    TEST_EQ(points[3].velocity.y, -1);

    int n_simulations = simulate(points, 1);
    TEST_EQ(n_simulations, 3);
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    auto lights = read_input(std::cin);
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    auto answer = simulate(lights, 1);

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

