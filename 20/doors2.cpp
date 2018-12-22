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

struct Coord {
    int i, j;
    bool operator<(const Coord& o) const {
        return std::tie(i, j) < std::tie(o.i, o.j);
    }
    Coord make(char c) {
        switch (c) {
        case 'N': return Coord{i-1, j};
        case 'E': return Coord{i, j+1};
        case 'S': return Coord{i+1, j};
        case 'W': return Coord{i, j-1};
        }
        throw "Bad input";
    }
};

using Graph = std::map<Coord, std::set<Coord>>;

void build_graph(auto& it, Graph& graph, Coord current) {
    Coord init = current;
    while (*it != ')') {
        if (*it == '|') {
            current = init;
        }
        else if (*it == '(') {
            build_graph(++it, graph, current);
        }
        else {
            Coord next = current.make(*it);
            graph[current].insert(next);
            current = next;
        }
        ++it;
    }
}

int bfs_furthest(Graph& graph) {
    std::set<Coord> visited;
    std::queue<std::pair<Coord, int>> q;
    q.push({Coord{0, 0}, 0});

    int answer = 0;

    while (!q.empty()) {
        auto el = q.front(); q.pop();
        if (visited.count(el.first)) continue;
        visited.insert(el.first);
        answer = std::max(el.second, answer);
        for (const auto& c : graph[el.first]) {
            q.push({c, el.second + 1});
        }
    }
    return answer;
}

int bfs(Graph& graph) {
    std::set<Coord> visited;
    std::queue<std::pair<Coord, int>> q;
    q.push({Coord{0, 0}, 0});

    int answer = 0;

    while (!q.empty()) {
        auto el = q.front(); q.pop();
        if (visited.count(el.first)) continue;
        visited.insert(el.first);
        if (el.second > 999) ++answer;
        for (const auto& c : graph[el.first]) {
            q.push({c, el.second + 1});
        }
    }
    return answer;
}

int furthest_door(std::string regex) {
    regex.front() = '(';
    regex.back() = ')';
    std::map<Coord, int> visited;
    auto it = ++begin(regex);

    Graph graph;
    build_graph(it, graph, Coord{0, 0});

    return bfs_furthest(graph);
}

int longest_path(std::string regex) {
    regex.front() = '(';
    regex.back() = ')';
    std::map<Coord, int> visited;
    auto it = ++begin(regex);

    Graph graph;
    build_graph(it, graph, Coord{0, 0});

    return bfs(graph);
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    TEST_EQ(furthest_door("^WNE$"), 3);
    TEST_EQ(furthest_door("^ENWWW(NEEE|SSE(EE|N))$"), 10);
    TEST_EQ(furthest_door("^ENNWSWW(NEWS|)SSSEEN(WNSE|)EE(SWEN|)NNN$"), 18);
    TEST_EQ(furthest_door("^WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWS(E|SS))))$"), 31);
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    std::string line; getline(std::cin, line);
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    auto answer = longest_path(line);

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

