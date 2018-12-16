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

using Register = std::array<int, 4>;
using Command = std::array<int, 4>;

struct Sample {
    Register before;
    Command cmd;
    Register after;
};

std::vector<Sample> read_samples(std::istream& is) {
    std::vector<Sample> samples;

    std::string line;
    getline(is, line);
    while (!line.empty()) {
        Sample s;
        std::string filler; char c;

        std::stringstream ss(line);
        ss >> filler >> c >> s.before[0] >> c >> s.before[1] >> c >> s.before[2] >> c >> s.before[3];
        getline(is, line);
        ss.str(line);
        ss >> s.cmd[0] >> s.cmd[1] >> s.cmd[2] >> s.cmd[3];
        getline(is, line);
        std::stringstream ss3(line);
        ss3 >> filler >> c >> s.after[0] >> c >> s.after[1] >> c >> s.after[2] >> c >> s.after[3];

        samples.push_back(s);

        getline(is, line);
        getline(is, line);
    }

    return samples;
}

std::vector<std::function<Register(Register, Command)>> all_commands {
    // addr
    [](Register r, Command cmd) { r[cmd[3]] = r[cmd[1]] + r[cmd[2]]; return r; },
    // addi
    [](Register r, Command cmd) { r[cmd[3]] = r[cmd[1]] + cmd[2]; return r; },
    // mulr
    [](Register r, Command cmd) { r[cmd[3]] = r[cmd[1]] * r[cmd[2]]; return r; },
    // muli
    [](Register r, Command cmd) { r[cmd[3]] = r[cmd[1]] * cmd[2]; return r; },
    // banr
    [](Register r, Command cmd) { r[cmd[3]] = r[cmd[1]] & r[cmd[2]]; return r; },
    // bani
    [](Register r, Command cmd) { r[cmd[3]] = r[cmd[1]] & cmd[2]; return r; },
    // borr
    [](Register r, Command cmd) { r[cmd[3]] = r[cmd[1]] | r[cmd[2]]; return r; },
    // bori
    [](Register r, Command cmd) { r[cmd[3]] = r[cmd[1]] | cmd[2]; return r; },
    // setr
    [](Register r, Command cmd) { r[cmd[3]] = r[cmd[1]]; return r; },
    // seti
    [](Register r, Command cmd) { r[cmd[3]] = cmd[1]; return r; },
    // gtir
    [](Register r, Command cmd) { r[cmd[3]] = cmd[1] > r[cmd[2]]; return r; },
    // gtri
    [](Register r, Command cmd) { r[cmd[3]] = r[cmd[1]] > cmd[2]; return r; },
    // gtrr
    [](Register r, Command cmd) { r[cmd[3]] = r[cmd[1]] > r[cmd[2]]; return r; },
    // eqir
    [](Register r, Command cmd) { r[cmd[3]] = cmd[1] == r[cmd[2]]; return r; },
    // eqri
    [](Register r, Command cmd) { r[cmd[3]] = r[cmd[1]] == cmd[2]; return r; },
    // eqrr
    [](Register r, Command cmd) { r[cmd[3]] = r[cmd[1]] == r[cmd[2]]; return r; },
};

size_t num_with_more_than_3_interpretations(const std::vector<Sample>& samples) {
    size_t result = 0;
    for (const auto& sample : samples) {
        size_t num_interpretations = 0;
        for (const auto& op : all_commands) {
            if (sample.after == op(sample.before, sample.cmd)) ++num_interpretations;
        }
        if (num_interpretations > 2) ++result;
    }
    return result;
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    std::string input = ""
        "Before: [3, 2, 1, 1]\n"
        "9 2 1 2\n"
        "After:  [3, 2, 2, 1]\n"
        "\n"
        "Before: [3, 2, 1, 1]\n"
        "9 0 1 2\n"
        "After:  [3, 2, 2, 1]\n"
        "\n"
        "\n"
        "\n"
        "5 0 0 3\n"
        "4 3 2 3\n"
        "0 0 3 1\n";
    std::stringstream ss(input);

    auto samples = read_samples(ss);
    TEST_EQ(samples.size(), 2);
    TEST_EQ(samples[0].before, (Register{3, 2, 1, 1}));
    TEST_EQ(samples[0].cmd, (Command{9, 2, 1, 2}));
    TEST_EQ(samples[0].after, (Register{3, 2, 2, 1}));

    TEST_EQ(num_with_more_than_3_interpretations(samples), 1);
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    auto samples = read_samples(std::cin);
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    auto answer = num_with_more_than_3_interpretations(samples);

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

