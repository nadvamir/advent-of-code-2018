#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstring>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>

bool opposite(char c1, char c2) {
    if (c1 >= 'a') return ('a' + (c2 - 'A')) == c1;
    else return ('A' + (c2 - 'a')) == c1;
}

long reduce_polymer(const std::string& polymer_str) {
    std::list<char> polymer(begin(polymer_str), end(polymer_str));

    auto current = begin(polymer); ++current;
    auto prev = begin(polymer);
    while (current != end(polymer) && prev != current) {
        if (opposite(*prev, *current)) {
            current = polymer.erase(current);
            prev = polymer.erase(prev);
            if (prev != begin(polymer)) --prev;
            if (current == begin(polymer) && current != end(polymer)) ++current;
        } else {
            ++prev;
            ++current;
        }
    }
    return polymer.size();
}

std::string remove_chars(std::string polymer, char c1, char c2) {
    polymer.erase(remove_if(begin(polymer), end(polymer), [=](char c) {
        return c == c1 || c == c2;
    }),
    end(polymer));
    return polymer;
}

long max_reduced(const std::string& polymer_str) {
    long result = polymer_str.size();
    for (int i = 0; i < 26; ++i) {
        result = std::min(result, reduce_polymer(remove_chars(polymer_str, 'a' + i, 'A' + i)));
    }
    return result;
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    TEST_EQ(opposite('c', 'C'), true);
    TEST_EQ(opposite('C', 'c'), true);
    TEST_EQ(opposite('Z', 'z'), true);
    TEST_EQ(opposite('h', 'H'), true);
    TEST_EQ(opposite('c', 'c'), false);

    TEST_EQ(reduce_polymer("aA"), 0);
    TEST_EQ(reduce_polymer("abBA"), 0);
    TEST_EQ(reduce_polymer("aBAb"), 4);
    TEST_EQ(reduce_polymer("aaBAAb"), 6);
    TEST_EQ(reduce_polymer("dabAcCaCBAcCcaDA"), 10);
    TEST_EQ(reduce_polymer("ZzzFhHfZXxLeEZzMyLlKkYmAaRtTtoOEeeELlySsTEetJjYyJjYzZTrlAatalPpLA"), 1);

    TEST_EQ(remove_chars("abBACdaCA", 'a', 'A'), "bBCdC");

    TEST_EQ(max_reduced("dabAcCaCBAcCcaDA"), 4);
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    std::string line;
    getline(std::cin, line);
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    const auto answer = max_reduced(line);

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

