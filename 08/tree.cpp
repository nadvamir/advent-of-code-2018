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

int sum_metadata(const Node& n) {
    int sum = std::accumulate(begin(n.metadata), end(n.metadata), 0);
    for (const auto& child : n.nodes) {
        sum += sum_metadata(*child);
    }
    return sum;
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    std::stringstream ss("2 3 0 3 10 11 12 1 1 0 1 99 2 1 1 2");

    const auto tree = build_tree(ss);

    TEST_EQ(sum_metadata(*tree), 138);
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    const auto tree = build_tree(std::cin);
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    const auto answer = sum_metadata(*tree);

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

