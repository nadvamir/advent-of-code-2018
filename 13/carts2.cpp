#include <algorithm>
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

enum class Direction {
    N = 0,
    E = 1,
    S = 2,
    W = 3,
};

enum class TurnDirection {
    LEFT = 0,
    STRAIGHT = 1,
    RIGHT = 2
};

struct Cart {
    int i, j;
    Direction direction;
    TurnDirection next_turn;
    bool crashed;

    void on_intersection() {
        if (next_turn == TurnDirection::RIGHT) direction = (Direction)((int(direction) + 1) % 4);
        else if (next_turn == TurnDirection::LEFT) direction = (Direction)((4 + int(direction) - 1) % 4);
        next_turn = (TurnDirection)((int(next_turn) + 1) % 3);
    }

    void move(const std::vector<std::string>& tracks) {
        switch (direction) {
            case Direction::N: --i; break;
            case Direction::E: ++j; break;
            case Direction::S: ++i; break;
            case Direction::W: --j; break;
        }

        if (tracks[i][j] == '/') {
            if (direction == Direction::N) direction = Direction::E;
            else if (direction == Direction::E) direction = Direction::N;
            else if (direction == Direction::W) direction = Direction::S;
            else if (direction == Direction::S) direction = Direction::W;
        }
        else if (tracks[i][j] == '\\') {
            if (direction == Direction::N) direction = Direction::W;
            else if (direction == Direction::E) direction = Direction::S;
            else if (direction == Direction::W) direction = Direction::N;
            else if (direction == Direction::S) direction = Direction::E;
        }
        else if (tracks[i][j] == '+') {
            on_intersection();
        } else if (tracks[i][j] != '-' && tracks[i][j] != '|'){
            std::cout << "What is happening?" << tracks[i][j] << ';' << std::endl;
            throw "WAAAT";
        }
    }

    static bool is_cart(char c) {
        return c == '^' || c == '>' || c == 'v' || c == '<';
    }

    static Cart make(int i, int j, char c) {
        switch(c) {
            case '^': return {i, j, Direction::N, TurnDirection::LEFT, false};
            case '>': return {i, j, Direction::E, TurnDirection::LEFT, false};
            case 'v': return {i, j, Direction::S, TurnDirection::LEFT, false};
            case '<': return {i, j, Direction::W, TurnDirection::LEFT, false};
        }
    }

    static char get_track_bit(char c) {
        switch(c) {
            case '^': case 'v': return '|';
            default: return '-';
        }
    }
};

std::vector<Cart> parse_carts(std::vector<std::string>& tracks) {
    std::vector<Cart> carts;
    for (int i = 0; i < tracks.size(); ++i) {
        for (int j = 0; j < tracks[i].size(); ++j) {
            char c = tracks[i][j];
            if (Cart::is_cart(c)) {
                carts.push_back(Cart::make(i, j, c));
                tracks[i][j] = Cart::get_track_bit(c);
            }
        }
    }
    return carts;
}

std::string simulate(std::vector<Cart>& carts, std::vector<std::string>& tracks) {
    std::map<std::pair<int, int>, Cart*> coords;
    for (auto& cart: carts) {
        coords.insert({{cart.i, cart.j}, &cart});
    }

    while (coords.size() != 1) {
        std::vector<Cart*> order;
        for (const auto& coord: coords) order.push_back(coord.second);

        for (auto c: order) {
            auto& cart = *c;
            if (cart.crashed) continue;

            coords.erase({cart.i, cart.j});
            cart.move(tracks);

            if (coords.count({cart.i, cart.j})) {
                coords[{cart.i, cart.j}]->crashed = true;
                cart.crashed = true;
                coords.erase({cart.i, cart.j});
            } else {
                coords.insert({{cart.i, cart.j}, &cart});
            }
        }
    }

    for (auto& cart: carts) {
        if (!cart.crashed) {
            return std::to_string(cart.j) + "," + std::to_string(cart.i);
        }
    }
    return "Nope";
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    std::vector<std::string> tracks {
        "/>-<\\  ",
        "|   |  ",
        "| /<+-\\",
        "| | | v",
        "\\>+</ |",
        "  |   ^",
        "  \\<->/",
    };

    auto carts = parse_carts(tracks);
    auto last = simulate(carts, tracks);

    TEST_EQ(last, "6,4");

    Cart cart{5, 5, Direction::N, TurnDirection::LEFT, false};

}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    std::vector<std::string> tracks;
    std::string line;
    while (getline(std::cin, line)) tracks.push_back(line);
    auto carts = parse_carts(tracks);
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    auto answer = simulate(carts, tracks);

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

