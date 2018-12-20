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

int longest_path_(auto& it) {
    int path = 0, longest_path = 0;
    while (*it != ')') {
        if (*it == '|') {
            longest_path = std::max(path, longest_path);
            path = -1;
        }
        else if (*it == '(') {
            path += longest_path_(++it) - 1;
        }
        ++it;
        ++path;
    }
    if (path == 0) return 0;
    return std::max(path, longest_path);
}

int longest_path(std::string regex) {
    regex.front() = '(';
    regex.back() = ')';
    auto it = ++begin(regex);
    return longest_path_(it);
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    TEST_EQ(longest_path("^WNE$"), 3);
    TEST_EQ(longest_path("^ENWWW(NEEE|SSE(EE|N))$"), 10);
    TEST_EQ(longest_path("^ENNWSWW(NEWS|)SSSEEN(WNSE|)EE(SWEN|)NNN$"), 18);
    TEST_EQ(longest_path("^WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWS(E|SS))))$"), 31);
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    std::string line; getline(std::cin, line);
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    auto answer = longest_path(line);

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

