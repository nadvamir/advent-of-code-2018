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

const int SIZE = 300;

int calc_power(int x, int y, int serial_n) {
    int rack_id = x + 10;
    int power_level = (rack_id * y + serial_n) * rack_id;
    int hundreds = power_level % 1000 / 100;
    return hundreds - 5;
}

std::string get_max_coords(int serial_n) {
    int grid[SIZE][SIZE] = {0};
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            grid[i][j] = calc_power(j, i, serial_n);
        }
    }

    auto sum_power = [&](int i, int j) {
        int sum = 0;
        for (int ii = 0; ii < 3; ++ii) {
            for (int jj = 0; jj < 3; ++jj) {
                sum += grid[i+ii][j+jj];
            }
        }
        return sum;

    };

    int max_i = 0, max_j = 0, max_sum = sum_power(0, 0);
    for (int i = 0; i + 3 < SIZE; ++i) {
        for (int j = 0; j + 3 < SIZE; ++j) {
            int sum = sum_power(i, j);
            if (sum > max_sum) {
                max_sum = sum;
                max_i = i;
                max_j = j;
            }
        }
    }

    return std::to_string(max_j) + "," + std::to_string(max_i);
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    TEST_EQ(calc_power(122, 79, 57), -5)
    TEST_EQ(calc_power(217, 196, 39), 0);
    TEST_EQ(calc_power(101, 153, 71), 4);
    TEST_EQ(get_max_coords(18), "33,45");
    TEST_EQ(get_max_coords(42), "21,61");
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    int serial_n; std::cin >> serial_n;
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    auto answer = get_max_coords(serial_n);

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

