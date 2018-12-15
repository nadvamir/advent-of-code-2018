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

using Coord = std::pair<int, int>;

Coord operator+(const Coord& c1, const Coord& c2) {
    return {c1.first + c2.first, c1.second + c2.second};
}

struct Unit {
    int hp {200}, ap{3};
    bool is_elve{false};
    Coord coords;
    char get_char() {
        return is_elve ? 'E' : 'G';
    }
    static Unit* make_elve(int i, int j) {
        return new Unit{200, 3, true, {i, j}};
    }
    static Unit* make_goblin(int i, int j) {
        return new Unit{200, 3, false, {i, j}};
    }
};

std::map<Coord, Unit*> make_units(const std::vector<std::string>& cave) {
    std::map<Coord, Unit*> units;
    for (size_t i = 0; i < cave.size(); ++i) {
        for (size_t j = 0; j < cave[i].size(); ++j) {
            if (cave[i][j] == 'E') units.insert({{i, j}, Unit::make_elve(i, j)});
            else if (cave[i][j] == 'G') units.insert({{i, j}, Unit::make_goblin(i, j)});
        }
    }
    return units;
}

auto get_moving_order(std::map<Coord, Unit*>& units) {
    std::vector<Unit*> order;
    for (auto& unit: units) {
        order.push_back(unit.second);
    }
    return order;
}

std::vector<Coord> get_target_coord(
        Coord start,
        bool is_elve,
        std::vector<std::string> cave,
        std::map<Coord, Unit*>& units) {
    char target = is_elve ? 'G' : 'E';

    std::queue<std::vector<Coord>> q;
    std::vector<Coord> s{start};
    q.push(s);

    std::map<int, std::vector<Coord>> targets;

    std::vector<Coord> moves {
        {-1, 0}, {0, -1}, {0, 1}, {1, 0}
    };
    while (!q.empty()) {
        auto path = std::move(q.front());
        q.pop();
        auto cur = path.back();

        if (!targets.empty()) break;

        for (const auto move: moves) {
            auto next = cur + move;
            auto& t = cave[next.first][next.second];
            if (t == '#') continue;
            if (t == '.') {
                t = '#';
                auto next_path = path;
                next_path.push_back(next);
                q.emplace(std::move(next_path));
            }
            else if (t == target) {
                auto next_path = path;
                next_path.push_back(next);
                next_path.erase(begin(next_path));
                if (path.size() <= 2) {
                    targets.insert({units[next]->hp, next_path});
                }
                else {
                    return next_path;
                }
            }
        }
    }

    if (!targets.empty()) {
        return begin(targets)->second;
    }

    return {};
}

size_t fight(std::vector<std::string> cave) {
    bool stop = false;

    std::map<Coord, Unit*> units = make_units(cave);
    // std::cout << "Initially:" << std::endl;
    // for (const auto& l: cave) {
    //     std::cout << l << std::endl;
    // }
    // std::cout << std::endl;

    size_t round = 0;
    for (; !stop; ++round) {
        auto moving_order = get_moving_order(units);
        stop = false;

        for (auto unit: moving_order) {
            if (unit->hp <= 0) continue;

            int num_elves = 0, num_goblins = 0;
            for (auto unit: moving_order) {
                if (unit->hp > 0) {
                    if (unit->is_elve) ++num_elves;
                    else ++num_goblins;
                }
            }
            if (num_elves == 0 || num_goblins == 0) {
                stop = true;
                break;
            }

            std::vector<Coord> go_to = get_target_coord(unit->coords, unit->is_elve, cave, units);
            if (go_to.empty()) continue;
            if (go_to.size() > 1) {
                // go
                units.erase(unit->coords);
                cave[unit->coords.first][unit->coords.second] = '.';
                unit->coords = go_to.front();
                cave[unit->coords.first][unit->coords.second] = unit->get_char();
                units.insert({unit->coords, unit});
            }
            if (go_to.size() <= 2) {
                // attack
                units[go_to.back()]->hp -= unit->ap;
                if (units[go_to.back()]->hp <= 0) {
                    cave[go_to.back().first][go_to.back().second] = '.';
                    units.erase(go_to.back());
                }
            }
        }

        for (auto unit: moving_order) {
            if (unit->hp <= 0) delete unit;
        }

        // std::cout << "After " << (round+1) << " rounds:" << std::endl;
        // for (const auto& l: cave) {
        //     std::cout << l << std::endl;
        // }
        // for (const auto& u: units) {
        //     std::cout << (u.second->is_elve ? 'E' : 'G') << ": " << u.second->hp << std::endl;
        // }
        // std::cout << std::endl;
    }

    size_t hp = 0;
    for (const auto& unit: units) {
        hp += unit.second->hp;
    }

    --round;

    std::cout << round << " * " << hp << std::endl;

    return round * hp;
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    std::vector<std::string> cave1 {
        "#######",
        "#.G...#",
        "#...EG#",
        "#.#.#G#",
        "#..G#E#",
        "#.....#",
        "#######",
    };
    TEST_EQ(fight(cave1), 27730);

    std::vector<std::string> cave2 {
        "#######",
        "#G..#E#",
        "#E#E.E#",
        "#G.##.#",
        "#...#E#",
        "#...E.#",
        "#######",
    };
    TEST_EQ(fight(cave2), 36334);

    std::vector<std::string> cave3 {
        "#######",
        "#E..EG#",
        "#.#G.E#",
        "#E.##E#",
        "#G..#.#",
        "#..E#.#",
        "#######",
    };
    TEST_EQ(fight(cave3), 39514);

    std::vector<std::string> cave4 {
        "#######",
        "#E.G#.#",
        "#.#G..#",
        "#G.#.G#",
        "#G..#.#",
        "#...E.#",
        "#######",
    };
    TEST_EQ(fight(cave4), 27755);

    std::vector<std::string> cave5 {
        "#######",
        "#.E...#",
        "#.#..G#",
        "#.###.#",
        "#E#G#G#",
        "#...#G#",
        "#######",
    };
    TEST_EQ(fight(cave5), 28944);

    std::vector<std::string> cave6 {
        "#########",
        "#G......#",
        "#.E.#...#",
        "#..##..G#",
        "#...##..#",
        "#...#...#",
        "#.G...G.#",
        "#.....G.#",
        "#########",
    };
    TEST_EQ(fight(cave6), 18740);
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    std::vector<std::string> cave;
    std::string line;
    while (std::getline(std::cin, line)) cave.push_back(line);
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    auto answer = fight(cave);

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

