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

unsigned long long calc_score(int n_players, int last_marble) {
    const int MAX_PLAYERS = 500;
    unsigned long long players[MAX_PLAYERS] = {0};

    std::list<int> circle {0};
    auto current_marble = begin(circle);

    auto play_23 = [&](int marble, int player) {
        for (int i = 0; i < 7; ++i) {
            if (current_marble == begin(circle)) {
                current_marble = end(circle);
            }
            --current_marble;
        }

        players[player] += marble + *current_marble;
        current_marble = circle.erase(current_marble);
    };

    auto place_marble = [&](int marble) {
        ++current_marble;
        if (current_marble == end(circle)) {
            current_marble = begin(circle);
        }
        ++current_marble;
        current_marble = circle.insert(current_marble, marble);
    };

    for (int marble = 1, player = 1; marble <= last_marble; ++marble, player = (player + 1) % n_players) {
        if (marble % 23 == 0) {
            play_23(marble, player);
        } else {
            place_marble(marble);
        }
    }

    return *std::max_element(players, players + MAX_PLAYERS);
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    TEST_EQ(calc_score(9, 25), 32);
    TEST_EQ(calc_score(10, 1618), 8317);
    TEST_EQ(calc_score(13, 7999), 146373);
    TEST_EQ(calc_score(17, 1104), 2764);
    TEST_EQ(calc_score(21, 6111), 54718);
    TEST_EQ(calc_score(30, 5807), 37305);
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    int n_players, last_marble;
    std::string filler;
    std::cin >> n_players >> filler >> filler >> filler >> filler >> filler >> last_marble;
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    const auto answer = calc_score(n_players, last_marble);

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

