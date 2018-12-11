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

    std::map<std::tuple<int, int, int>, int> sums;

    auto sum_power = [&](int i, int j, int n) {
        if (n == 1) return grid[i][j];

        int sum = (n > 2) ? sums[{i, j, n-1}] : grid[i][j];

        for (int ii = 0; ii < n; ++ii) {
            sum += grid[i + ii][j + n - 1];
        }
        for (int jj = 0; jj + 1 < n; ++jj) {
            sum += grid[i + n - 1][j + jj];
        }

        sums[{i, j, n}] = sum;
        return sum;

    };

    int max_i = 0, max_j = 0, max_sum = sum_power(0, 0, 1);
    int max_n = 1;
    for (int n = 1; n <= 300; ++n) {
        for (int i = 0; i + n <= SIZE; ++i) {
            for (int j = 0; j + n <= SIZE; ++j) {
                int sum = sum_power(i, j, n);
                if (sum > max_sum) {
                    max_sum = sum;
                    max_i = i;
                    max_j = j;
                    max_n = n;
                }
            }
        }
    }

    return std::to_string(max_j) + "," + std::to_string(max_i) + "," + std::to_string(max_n);
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    TEST_EQ(calc_power(122, 79, 57), -5)
    TEST_EQ(calc_power(217, 196, 39), 0);
    TEST_EQ(calc_power(101, 153, 71), 4);
    auto coords = get_max_coords(18);
    TEST_EQ(coords, "90,269,16");
    coords = get_max_coords(42);
    TEST_EQ(coords, "232,251,12");
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

