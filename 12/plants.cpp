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

const size_t ZERO = 200;
const size_t MAX_N = ZERO * 2;
const size_t PATTERN_N = 5;

std::bitset<MAX_N> parse_plants(const std::string& plants) {
    std::bitset<MAX_N> bits;
    for (size_t i = 0; i < plants.size(); ++i) {
        if (plants[i] == '#') {
            bits.set(ZERO + i);
        }
    }
    return bits;
}

std::string to_binary(std::string pattern) {
    replace(begin(pattern), end(pattern), '.', '0');
    replace(begin(pattern), end(pattern), '#', '1');
    return pattern;
}

std::unordered_set<std::bitset<PATTERN_N>> parse_patterns(std::istream& is) {
    std::unordered_set<std::bitset<PATTERN_N>> patterns;
    std::string pattern, arrow;
    char is_set;
    while (is >> pattern >> arrow >> is_set) {
        if (is_set == '#') {
            patterns.insert(std::bitset<PATTERN_N>(to_binary(pattern)));
        }
    }
    return patterns;
}

int calc_checksum(const std::bitset<MAX_N> plants) {
    int sum = 0;
    for (size_t i = 0; i < MAX_N; ++i) {
        if (plants.test(i)) {
            sum += (i - ZERO);
        }
    }
    return sum;
}

int simulate(std::bitset<MAX_N> plants,
             const std::unordered_set<std::bitset<PATTERN_N>>& patterns,
             int generations) {
    for (size_t i = 95; i < 190; ++i) {
        std::cout << (plants[i] ? '#' : '.');
    }
    std::cout << std::endl;
    std::bitset<MAX_N> new_gen;
    std::bitset<PATTERN_N> pattern;
    while (generations--) {
        new_gen.reset();
        for (size_t i = 0; i + PATTERN_N < MAX_N; ++i) {
            for (size_t j = 0; j < PATTERN_N; ++j) {
                pattern[PATTERN_N - j - 1] = plants[i + j];
            }
            if (patterns.count(pattern)) {
                // std::cout << i << ":  " << pattern << std::endl;
                // for (size_t j = i; j < i + PATTERN_N; ++j) std::cout << plants[j];
                // std::cout << std::endl;
                new_gen.set(i + PATTERN_N/2);
            }
        }
        plants = new_gen;
        for (size_t i = 95; i < 190; ++i) {
            std::cout << (plants[i] ? '#' : '.');
        }
        std::cout << std::endl;
    }

    return calc_checksum(plants);
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    std::string input = "initial state: #..#.#..##......###...###\n"
        "\n"
        "...## => #\n"
        "..#.. => #\n"
        ".#... => #\n"
        ".#.#. => #\n"
        ".#.## => #\n"
        ".##.. => #\n"
        ".#### => #\n"
        "#.#.# => #\n"
        "#.### => #\n"
        "##.#. => #\n"
        "##.## => #\n"
        "###.. => #\n"
        "###.# => #\n"
        "####. => #\n";
    std::stringstream ss(input);
    std::string filler, pattern;
    ss >> filler >> filler >> pattern;

    auto plants = parse_plants(pattern);
    auto patterns = parse_patterns(ss);
    TEST_EQ(patterns.size(), 14);

    std::bitset<PATTERN_N> p1("00011"), p2("10001");
    TEST_EQ(patterns.count(p1), 1);
    TEST_EQ(patterns.count(p2), 0);

    auto result = simulate(plants, patterns, 20);
    std::cout << result << std::endl;
    TEST_EQ(result, 325);
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    std::string filler, pattern;
    std::cin >> filler >> filler >> pattern;
    auto plants = parse_plants(pattern);
    auto patterns = parse_patterns(std::cin);
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    auto answer = simulate(plants, patterns, 20);

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

