#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iterator>
#include <cassert>

long calibrated_freq(std::istream& is) {
    std::vector<long> data;
    copy(std::istream_iterator<long>(is), std::istream_iterator<long>(), back_inserter(data));

    std::unordered_set<long> repeat_freqs;
    long freq = 0;
    size_t i = 0;

    do {
        repeat_freqs.insert(freq);
        freq += data[i++ % data.size()];
    } while (!repeat_freqs.count(freq));

    return freq;
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    std::istringstream in1(std::string{"+1\n-2\n+3\n+1"});
    const long freq = calibrated_freq(in1);
    TEST_EQ(freq, 2);
}

void run() {
    std::cout << calibrated_freq(std::cin) << "\n";
}

int main() {
    test();
    run();
}

