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

using Register = std::array<int, 6>;
struct Command {
    std::string name;
    std::array<int, 3> p;
};

int IP = 0;

std::unordered_map<std::string, std::function<Register(Register, Command)>> all_commands {
    {"addr", [](Register r, Command cmd) { r[cmd.p[2]] = r[cmd.p[0]] + r[cmd.p[1]]; return r; }},
    {"addi", [](Register r, Command cmd) { r[cmd.p[2]] = r[cmd.p[0]] + cmd.p[1]; return r; }},
    {"mulr", [](Register r, Command cmd) { r[cmd.p[2]] = r[cmd.p[0]] * r[cmd.p[1]]; return r; }},
    {"muli", [](Register r, Command cmd) { r[cmd.p[2]] = r[cmd.p[0]] * cmd.p[1]; return r; }},
    {"banr", [](Register r, Command cmd) { r[cmd.p[2]] = r[cmd.p[0]] & r[cmd.p[1]]; return r; }},
    {"bani", [](Register r, Command cmd) { r[cmd.p[2]] = r[cmd.p[0]] & cmd.p[1]; return r; }},
    {"borr", [](Register r, Command cmd) { r[cmd.p[2]] = r[cmd.p[0]] | r[cmd.p[1]]; return r; }},
    {"bori", [](Register r, Command cmd) { r[cmd.p[2]] = r[cmd.p[0]] | cmd.p[1]; return r; }},
    {"setr", [](Register r, Command cmd) { r[cmd.p[2]] = r[cmd.p[0]]; return r; }},
    {"seti", [](Register r, Command cmd) { r[cmd.p[2]] = cmd.p[0]; return r; }},
    {"gtir", [](Register r, Command cmd) { r[cmd.p[2]] = cmd.p[0] > r[cmd.p[1]]; return r; }},
    {"gtri", [](Register r, Command cmd) { r[cmd.p[2]] = r[cmd.p[0]] > cmd.p[1]; return r; }},
    {"gtrr", [](Register r, Command cmd) { r[cmd.p[2]] = r[cmd.p[0]] > r[cmd.p[1]]; return r; }},
    {"eqir", [](Register r, Command cmd) { r[cmd.p[2]] = cmd.p[0] == r[cmd.p[1]]; return r; }},
    {"eqri", [](Register r, Command cmd) { r[cmd.p[2]] = r[cmd.p[0]] == cmd.p[1]; return r; }},
    {"eqrr", [](Register r, Command cmd) { r[cmd.p[2]] = r[cmd.p[0]] == r[cmd.p[1]]; return r; }},
};

std::vector<Command> read_program(std::istream& is) {
    std::string filler;
    is >> filler >> IP;

    std::vector<Command> commands;
    Command c;
    while (is >> c.name >> c.p[0] >> c.p[1] >> c.p[2]) commands.push_back(c);
    return commands;
}

int execute(const std::vector<Command>& cmds, Register init) {
    int i = 0;
    std::set<Register> seen;
    while (init[IP] < cmds.size()) {
        if (i++ > 10000) break;
        seen.insert(init);

        std::cout << init[IP] << ": ";
        for (auto c : init) std::cout << c << " ";
        std::cout << " -> ";
        auto cmd = cmds[init[IP]];
        init = all_commands[cmd.name](init, cmd);
        for (auto c : init) std::cout << c << " ";
        std::cout << std::endl;
        ++init[IP];
    }
    --init[IP];
    return i;
}

int value_of_zero(const std::vector<Command>& cmds) {
    int mini = 0, minn = 1 << 10;
    for (int i = 0; i <= 0; ++i) {
        // taken from the output
        int n_commands = execute(cmds, Register{10720163, 0, 0, 0, 0, 0});
        // std::cout << i << " " << n_commands << std::endl;
        if (n_commands < minn) {
            minn = n_commands;
            mini = i;
        }
    }
    return mini;
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    auto commands = read_program(std::cin);
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    auto answer = value_of_zero(commands);

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

