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

using Coord = std::pair<int, int>;

std::vector<Coord> extract_clay_positions(const std::vector<std::string>& clay_descr) {
    std::vector<Coord> positions;

    char axis, filler, other_axis;
    int fixed_coord, from_coord, to_coord;
    for (const auto& descr : clay_descr) {
        std::stringstream ss(descr);
        ss >> axis >> filler >> fixed_coord >> filler
           >> other_axis >> filler >> from_coord >> filler >> filler >> to_coord;
        for (int i = from_coord; i <= to_coord; ++i) {
            if (axis == 'x') {
                positions.emplace_back(Coord{i, fixed_coord});
            }
            else {
                positions.emplace_back(Coord{fixed_coord, i});
            }
        }
    }

    return positions;
}

std::vector<std::string> create_scan(const std::vector<Coord>& clay_pos, int& minI) {
    minI = 1000;
    int maxI = 0, minJ = 501, maxJ = 499;
    for (const auto& clay : clay_pos) {
        if (clay.first > maxI) maxI = clay.first;
        else if (clay.first < minI) minI = clay.first;
        if (clay.second > maxJ) maxJ = clay.second;
        else if (clay.second < minJ) minJ = clay.second;
    }
    minJ -= 3; maxJ += 3;

    const int startX = (500 - minJ > maxJ - 500) ? minJ : 1000 - maxJ;
    const int size = (500 - startX) * 2;
    
    std::vector<std::string> scan;
    for (int i = 0; i <= maxI; ++i) {
        scan.emplace_back(std::string(size, '.'));
    }

    for (const auto& clay : clay_pos) {
        scan[clay.first][clay.second - startX] = '#';
    }
    return scan;
}

bool flow_water(std::vector<std::string>& scan) {
    auto go_left = [&](Coord c) {
        while (c.second > 0 && scan[c.first][c.second] != '#') {
            scan[c.first][c.second] = '|';
            if ((scan[c.first + 1][c.second] == '.' || scan[c.first + 1][c.second]  == '|')) break;
            --c.second;
        }
        return c;
    };
    auto go_right = [&](Coord c) {
        while (c.second + 1 < scan[c.first].size() && scan[c.first][c.second] != '#') {
            scan[c.first][c.second] = '|';
            if ((scan[c.first + 1][c.second] == '.' || scan[c.first + 1][c.second]  == '|')) break;
            ++c.second;
        }
        return c;
    };
    auto fill_with_stale = [&](Coord from, Coord to) {
        for (++from.second; from != to; ++from.second) {
            scan[from.first][from.second] = '~';
        }
    };

    bool added_stale_water = false;
    std::queue<Coord> q{{Coord{0, scan[0].size() / 2}}};
    while (!q.empty()) {
        auto c = q.front();
        q.pop();
        const bool can_go_down = c.first + 1 < scan.size();
        if (!can_go_down) continue;

        char& bottom = scan[c.first + 1][c.second];
        if (bottom == '.' || bottom == '|') {
            bottom = '|';
            q.push(Coord{c.first + 1, c.second});
            continue;
        }

        // going sideways
        Coord leftmost = go_left(c);
        Coord rightmost = go_right(c);
        if (scan[leftmost.first][leftmost.second] == '#' && scan[rightmost.first][rightmost.second] == '#') {
            fill_with_stale(leftmost, rightmost);
            added_stale_water = true;
        }
        else {
            if (leftmost != c && scan[leftmost.first][leftmost.second] != '#') {
                q.push(leftmost);
            }
            if (rightmost != c && scan[rightmost.first][rightmost.second] != '#') {
                q.push(rightmost);
            }
        }
    }
    return added_stale_water;
}

size_t count_water(const std::vector<std::string>& scan, int minI) {
    size_t count = 0;
    for (const auto& line : scan) {
        if (minI-- > 0) continue; // skip start
        for (const auto c : line) {
            if (c == '~' || c == '|') ++count;
        }
    }
    return count;
}

size_t num_water_squares(std::vector<std::string> scan, int minI) {
    while (flow_water(scan));
    return count_water(scan, minI);
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    std::vector<std::string> clay_descr {
        "x=495, y=2..7",
        "y=7, x=495..501",
        "x=501, y=3..7",
        "x=498, y=2..4",
        "x=506, y=1..2",
        "x=498, y=10..13",
        "x=504, y=10..13",
        "y=13, x=498..504",
    };

    auto clay_pos = extract_clay_positions(clay_descr);
    TEST_EQ(clay_pos.size(), 38);

    int minI = 0;
    auto scan = create_scan(clay_pos, minI);
    TEST_EQ(minI, 1);
    TEST_EQ(scan.size(), 14);
    TEST_EQ(scan[0].size(), 18);

    auto answer = num_water_squares(scan, minI);
    TEST_EQ(answer, 57);
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    std::string line;
    std::vector<std::string> clay_descr;
    while (std::getline(std::cin, line)) clay_descr.push_back(line);
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    auto clay_pos = extract_clay_positions(clay_descr);
    int minI = 0;
    auto scan = create_scan(clay_pos, minI);
    auto answer = num_water_squares(scan, minI);
    std::cout << "After count" << std::endl;

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

