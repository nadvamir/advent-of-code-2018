#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iterator>
#include <cassert>

std::vector<size_t> calc_counts(const std::string& box) {
    std::vector<size_t> counts(26);
    for (const char c: box) ++counts[c - 'a'];
    return counts;
}

size_t checksum(std::vector<std::string> boxes) {
    size_t twos = 0, threes = 0;

    for (const auto& box : boxes) {
        const auto counts = calc_counts(box);
        if (find(begin(counts), end(counts), 2) != end(counts)) ++twos;
        if (find(begin(counts), end(counts), 3) != end(counts)) ++threes;
    }

    return twos * threes;
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    TEST_EQ(checksum({"abcdef", "bababc", "abbcde", "abcccd", "aabcdd", "abcdee", "ababab"}), 12);
}

void run() {
    std::vector<std::string> data(std::istream_iterator<std::string>(std::cin), std::istream_iterator<std::string>());
    std::cout << checksum(std::move(data)) << "\n";
}

int main() {
    test();
    run();
}

