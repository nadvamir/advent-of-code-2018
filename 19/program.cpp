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

Register execute(const std::vector<Command>& cmds, Register init) {
    while (init[IP] < cmds.size()) {
        auto cmd = cmds[init[IP]];
        init = all_commands[cmd.name](init, cmd);
        ++init[IP];
    }
    --init[IP];
    return init;
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    std::string input = ""
        "#ip 0\n"
        "seti 5 0 1\n"
        "seti 6 0 2\n"
        "addi 0 1 0\n"
        "addr 1 2 3\n"
        "setr 1 0 0\n"
        "seti 8 0 4\n"
        "seti 9 0 5\n";
    std::stringstream ss(input);

    auto commands = read_program(ss);
    Register init{0, 0, 0, 0, 0, 0};
    auto vals = execute(commands, init);
    TEST_EQ(vals, (Register{6, 5, 6, 0, 0, 9}));
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    auto commands = read_program(std::cin);
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    auto answer = execute(commands, Register{0, 0, 0, 0, 0, 0})[0];

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

