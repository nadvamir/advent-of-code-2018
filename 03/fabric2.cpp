#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstring>
#include <iostream>
#include <iterator>
#include <functional>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>

struct Rect {
    size_t id;
    size_t x, y, w, h;

    bool operator==(const Rect& o) const {
        return std::tie(id, x, y, w, h) == std::tie(o.id, o.x, o.y, o.w, o.h);
    }
};

Rect parse_rect(std::string line) {
    Rect r;
    char c;
    std::stringstream ss(line);

    ss >> c >> r.id >> c >> r.x >> c >> r.y >> c >> r.w >> c >> r.h;

    return r;
}

const size_t CANVAS_SIZE = 1000;

size_t nonoverlapping_patch_id(std::vector<Rect> rectangles) {
    short canvas[CANVAS_SIZE][CANVAS_SIZE];
    memset(canvas, 0, sizeof canvas);

    std::function<void(const Rect& rect)> erase_rect = [&](const Rect& rect) -> void {
        for (int i = rect.y; i < rect.y + rect.h; ++i)
            for (int j = rect.x; j < rect.x + rect.w; ++j) {
                if (canvas[i][j] == -1) continue;
                else if (canvas[i][j] != 0 && canvas[i][j] != rect.id) erase_rect(rectangles[canvas[i][j] - 1]);
                else canvas[i][j] = -1;
            }
    };

    auto fill_rect = [&](const Rect& rect) {
        for (int i = rect.y; i < rect.y + rect.h; ++i)
            for (int j = rect.x; j < rect.x + rect.w; ++j)
                if (canvas[i][j] == 0) canvas[i][j] = rect.id;
                else erase_rect(rect);

    };

    for (const auto& rect: rectangles) fill_rect(rect);

    size_t num_overlapping = 0;
    size_t num_others = 0;
    for (int i = 0; i < CANVAS_SIZE; ++i)
        for (int j = 0; j < CANVAS_SIZE; ++j)
            if (canvas[i][j] > 0)
                return canvas[i][j];

    return 0;
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    std::vector<Rect> rectangles;
    rectangles.emplace_back(parse_rect("#1 @ 1,3: 4x4"));
    rectangles.emplace_back(parse_rect("#2 @ 3,1: 4x4"));
    rectangles.emplace_back(parse_rect("#3 @ 5,5: 2x2"));

    TEST_EQ(rectangles[0], (Rect{1, 1, 3, 4, 4}));
    TEST_EQ(rectangles[1], (Rect{2, 3, 1, 4, 4}));
    TEST_EQ(rectangles[2], (Rect{3, 5, 5, 2, 2}));

    TEST_EQ(nonoverlapping_patch_id(rectangles), 3);
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    std::vector<Rect> rectangles;
    std::string line;
    while (getline(std::cin, line)) {
        rectangles.emplace_back(parse_rect(line));
    }
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    assert(rectangles.size() == 1323);
    assert(rectangles[1322] == (Rect{1323, 793, 626, 19, 20}));

    const auto answer = nonoverlapping_patch_id(std::move(rectangles));

    high_resolution_clock::time_point t_end = high_resolution_clock::now();
    auto io_duration = duration_cast<microseconds>(t_io - t_start).count();
    auto all_duration = duration_cast<microseconds>(t_end - t_start).count();

    std::cout << answer << " in " << all_duration << "ms (io: "
              << io_duration << "ms)" << std::endl;
}

int main() {
    test();
    run();
}

