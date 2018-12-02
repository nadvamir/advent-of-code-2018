#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

size_t diff(const std::string& box1, const std::string& box2) {
    size_t ndiff = 0;
    for (size_t i = 0; i < box1.size(); ++i) {
        if (box1[i] != box2[i]) ++ndiff;
    }
    return ndiff;
}

std::pair<std::string, std::string> boxes_diff_by_1(std::vector<std::string> boxes) {
    for (size_t i = 0; i < boxes.size() + 1; ++i) {
        for (size_t j = i + 1; j < boxes.size(); ++j) {
            if (diff(boxes[i], boxes[j]) == 1) {
                return {boxes[i], boxes[j]};
            }
        }
    }
    return {"", ""};
}

std::string common_chars(std::string first, std::string second) {
    std::ostringstream common;
    for (size_t i = 0; i < first.size(); ++i) {
        if (first[i] == second[i]) {
            common << first[i];
        }
    }
    return common.str();
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    const auto close = boxes_diff_by_1({
        "abcde", "fghij", "klmno", "pqrst", "fguij", "axcye", "wcxyz"
    });
    TEST_EQ(close.first, "fghij");
    TEST_EQ(close.second, "fguij");

    TEST_EQ(common_chars(close.first, close.second), "fgij");
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    std::vector<std::string> data(std::istream_iterator<std::string>(std::cin), std::istream_iterator<std::string>());
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    const auto close = boxes_diff_by_1(std::move(data));
    const auto answer = common_chars(close.first, close.second);
    assert(answer == "lujnogabetpmsydyfcovzixaw");

    high_resolution_clock::time_point t_end = high_resolution_clock::now();
    auto io_duration = duration_cast<microseconds>(t_io - t_start).count();
    auto all_duration = duration_cast<microseconds>(t_end - t_start).count();

    std::cout << answer << " in " << all_duration << "ms (io: "
              << io_duration << "ms)" << std::endl;
}

int main() {
    test();
    run();
}

