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

struct Node {
    std::vector<std::unique_ptr<Node>> nodes;
    std::vector<int> metadata;
    int meta_value{0};
};

std::unique_ptr<Node> build_tree(std::istream& is) {
    auto node = std::make_unique<Node>();
    int num_children, num_metadata, metadata;
    is >> num_children >> num_metadata;

    while (num_children--) {
        node->nodes.emplace_back(build_tree(is));
    }

    while (num_metadata--) {
        is >> metadata;
        node->metadata.push_back(metadata);
    }

    return node;
}

int calc_value(Node& n) {
    if (!n.nodes.empty()) {
        int val = 0;
        for (const int meta_idx : n.metadata) {
            if (meta_idx <= n.nodes.size()) {
                val += calc_value(*n.nodes[meta_idx - 1]);
            }
        }
        return val;
    } else {
        return std::accumulate(begin(n.metadata), end(n.metadata), 0);
    }
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    std::stringstream ss("2 3 0 3 10 11 12 1 1 0 1 99 2 1 1 2");

    auto tree = build_tree(ss);

    TEST_EQ(calc_value(*tree), 66);
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    auto tree = build_tree(std::cin);
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    const auto answer = calc_value(*tree);

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

