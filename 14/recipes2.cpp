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

size_t recipe_after(size_t n_elves, std::string recipe) {
    std::vector<size_t> elves(n_elves);
    std::iota(begin(elves), end(elves), 0);
    std::string board = "37";
    for (size_t iteration = 0; true; ++iteration) {
        size_t next = 0;
        for (const auto& elve: elves) {
            next += board[elve] - '0';
        }
        board += std::to_string(next);
        for (auto& elve: elves) {
            elve = (elve + (board[elve] - '0') + 1) % board.size();
        }

        if (iteration % 100000 == 0) {
            auto it = board.find(recipe);
            if (it != std::string::npos) {
                return it;
            }
        }
    }
    return board.size() - recipe.size();
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    TEST_EQ(recipe_after(2, "51589"), 9);
    TEST_EQ(recipe_after(2, "01245"), 5);
    TEST_EQ(recipe_after(2, "92510"), 18);
    TEST_EQ(recipe_after(2, "59414"), 2018);
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    std::string recipe; std::cin >> recipe;
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    auto answer = recipe_after(2, recipe);

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

