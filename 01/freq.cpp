#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

long calibrated_freq(std::istream& is) {
    long num, sum = 0;
    while (is >> num) {
        sum += num;
    }
    return sum;
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    std::istringstream in1(std::string{"+3\n-2\n+1\n+1"});
    const long freq = calibrated_freq(in1);
    TEST_EQ(freq, 3);
}

void run() {
    std::cout << calibrated_freq(std::cin) << "\n";
}

int main() {
    test();
    run();
}

