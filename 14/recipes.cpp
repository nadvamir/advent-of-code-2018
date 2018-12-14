#include <algorithm>
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

std::string next_10_after(size_t n_elves, size_t recipes) {
    std::vector<size_t> elves(n_elves);
    std::iota(begin(elves), end(elves), 0);
    std::string board = "37";
    while (board.size() < recipes + 10) {
        size_t next = 0;
        for (const auto& elve: elves) {
            next += board[elve] - '0';
        }
        board += std::to_string(next);
        for (auto& elve: elves) {
            elve = (elve + (board[elve] - '0') + 1) % board.size();
        }
    }
    return board.substr(recipes, 10);
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    TEST_EQ(next_10_after(2, 9), "5158916779");
    TEST_EQ(next_10_after(2, 5), "0124515891");
    TEST_EQ(next_10_after(2, 18), "9251071085");
    TEST_EQ(next_10_after(2, 2018), "5941429882");
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    size_t recipes; std::cin >> recipes;
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    auto answer = next_10_after(2, recipes);

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

