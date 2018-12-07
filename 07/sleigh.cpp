#include <algorithm>
#include <cassert>
#include <cmath>
#include <chrono>
#include <cstring>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
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

struct Vertex {
    char name;
    int degree{0};
    std::set<char> children;
};

std::map<char, Vertex> make_graph(const std::vector<std::string>& instructions) {
    std::map<char, Vertex> graph;
    for (const auto& line: instructions) {
        std::stringstream ss(line);
        char from, to;
        std::string filler;
        ss >> filler >> from >> filler >> filler >> filler >> filler >> filler >> to;

        if (!graph.count(from)) {
            graph.insert({from, {from, 0, {to}}});
        } else {
            graph[from].children.insert(to);
        }

        if (!graph.count(to)) {
            graph.insert({to, {to, 1, {}}});
        } else {
            ++graph[to].degree;
        }
    }
    return graph;
}

std::string order_instructions(const std::vector<std::string>& instructions) {
    auto graph = make_graph(instructions);

    std::priority_queue<char, std::vector<char>, std::greater<char>> zero_degree;
    for (const auto& v: graph) if (v.second.degree == 0) zero_degree.push(v.first);

    std::stringstream answer;
    while (!zero_degree.empty()) {
        auto& v = graph[zero_degree.top()];
        zero_degree.pop();
        answer << v.name;

        for (const char c: v.children) {
            auto& child = graph[c];
            --child.degree;
            if (child.degree == 0) {
                zero_degree.push(child.name);
            }
        }
    }

    return answer.str();
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    std::vector<std::string> instructions {
        "Step F must be finished before step E can begin.",
        "Step C must be finished before step A can begin.",
        "Step C must be finished before step F can begin.",
        "Step A must be finished before step B can begin.",
        "Step A must be finished before step D can begin.",
        "Step B must be finished before step E can begin.",
        "Step D must be finished before step E can begin.",
    };

    auto graph = make_graph(instructions);
    TEST_EQ(graph.size(), 6);
    TEST_EQ(graph['E'].degree, 3);
    TEST_EQ(graph['C'].degree, 0);
    TEST_EQ(graph['C'].children, (std::set<char>{'A', 'F'}));

    TEST_EQ(order_instructions(instructions), "CABDFE");
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    std::vector<std::string> instructions;
    std::string line;
    while (std::getline(std::cin, line)) instructions.push_back(line);
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    const auto answer = order_instructions(instructions);

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

