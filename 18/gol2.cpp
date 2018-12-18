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

using GOL = std::vector<std::string>;

char next_cell(const GOL& area, int i, int j) {
    size_t adjacent_trees = 0, adjacent_lumber = 0;
    auto inspect = [&](int ii, int jj) {
        if (ii >= 0 && ii < area.size() && jj >= 0 && jj < area[i].size()) {
            if (area[ii][jj] == '|') ++adjacent_trees;
            else if (area[ii][jj] == '#') ++adjacent_lumber;
        }
    };

    inspect(i - 1, j - 1);
    inspect(i - 1, j);
    inspect(i - 1, j + 1);
    inspect(i, j - 1);
    inspect(i, j + 1);
    inspect(i + 1, j - 1);
    inspect(i + 1, j);
    inspect(i + 1, j + 1);

    if (area[i][j] == '.' && adjacent_trees >= 3) return '|';
    if (area[i][j] == '|' && adjacent_lumber >= 3) return '#';
    if (area[i][j] == '#' && (adjacent_lumber == 0 || adjacent_trees == 0)) return '.';
    return area[i][j]; 
}

size_t calc_resources(const GOL& area) {
    size_t num_trees = 0, num_lumberyard = 0;
    for (const auto& line : area) {
        for (char plot : line) {
            if (plot == '|') ++num_trees;
            else if (plot == '#') ++num_lumberyard;
        }
    }
    return num_trees * num_lumberyard;
}

GOL simulate(GOL area, int num_sims) {
    std::unordered_map<size_t, std::pair<GOL, int>> previous;
    GOL another_area = area;
    GOL* current = &area;
    GOL* next = &another_area;
    while (num_sims--) {
        auto resources = calc_resources(*current);
        if (previous.count(resources)) {
            auto& prev = previous[resources];
            if (prev.first == *current) {
                auto diff = prev.second - num_sims;
                num_sims %= diff;
            }
        }
        previous[resources] = {*current, num_sims};

        for (size_t i = 0; i < area.size(); ++i) {
            for (size_t j = 0; j < area[i].size(); ++j) {
                (*next)[i][j] = next_cell(*current, i, j);
            }
        }
        std::swap(current, next);
    }
    for (const auto& line : *current) std::cout << line << std::endl;
    std::cout << std::endl;
    return *current;
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    std::vector<std::string> area {
        ".#.#...|#.",
        ".....#|##|",
        ".|..|...#.",
        "..|#.....#",
        "#.#|||#|#|",
        "...#.||...",
        ".|....|...",
        "||...#|.#|",
        "|.||||..|.",
        "...#.|..|.",
    };

    size_t answer = calc_resources(simulate(area, 10));
    TEST_EQ(answer, 1147);
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    std::string line;
    std::vector<std::string> area;
    while (std::getline(std::cin, line)) area.push_back(line);
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    size_t answer = calc_resources(simulate(area, 1000000000));

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

