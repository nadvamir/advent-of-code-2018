#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstring>
#include <iostream>
#include <iterator>
#include <numeric>
#include <functional>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>

struct Log {
    enum Type {
        BEGIN, FALL_ASLEEP, WAKE_UP
    };

    int year, month, day, hour, minute;
    Type type;
    int id {0};

    bool operator<(const Log& o) const {
        return std::tie(year, month, day, hour, minute) < std::tie(o.year, o.month, o.day, o.hour, o.minute);
    }

    bool operator==(const Log& o) const {
        return std::tie(year, month, day, hour, minute, type, id) == std::tie(o.year, o.month, o.day, o.hour, o.minute, o.type, o.id);
    }

    friend std::ostream& operator<<(std::ostream& out, const Log& l) {
        return out << l.year << " " << l.month << " " << l.day << ": "
                   << l.hour << ":" << l.minute << " - " << l.type << " " << l.id;
    }
};

struct Guard {
    int id;
    int sleeps[60] = {0};

    Guard() : id{0} {}
    Guard(int id) : id(id) {}

    void sleep(int from, int to) {
        while (from < to) ++sleeps[from++];
    }

    int time_awake() const {
        return std::accumulate(sleeps, sleeps+60, 0);
    }

    bool operator<(const Guard& o) const {
        return time_awake() < o.time_awake();
    }
};

Log parse_log(std::string line) {
    Log l;
    char c;
    std::string type;
    std::stringstream ss(line);

    ss >> c >> l.year >> c >> l.month >> c >> l.day
       >> l.hour >> c >> l.minute >> c >> type;

    if (type == "Guard") {
        ss >> c >> l.id;
        l.type = Log::BEGIN;
    } else if (type == "wakes") {
        l.type = Log::WAKE_UP;
    } else {
        l.type = Log::FALL_ASLEEP;
    }

    return l;
}

void assign_ids(std::vector<Log>& logs) {
    int id = 0;
    for (auto& log : logs) {
        if (log.type == Log::BEGIN) id = log.id;
        else log.id = id;
    }
}

std::unordered_map<int, Guard> create_guards_map(const std::vector<Log>& logs) {
    std::unordered_map<int, Guard> guards;
    int from = 0;

    for (auto& log : logs) {
        if (log.type == Log::BEGIN) {
            if (!guards.count(log.id)) {
                guards.insert({log.id, {log.id}});
            }
        } else if (log.type == Log::FALL_ASLEEP) {
            from = log.minute;
        } else {
            guards[log.id].sleep(from, log.minute);
        }
    }

    return guards;
}

int minute_with_strat_1(std::vector<Log> logs) {
    const auto guards = create_guards_map(logs);
    Guard least_woke;
    for (const auto& g : guards) if (least_woke < g.second) least_woke = g.second;
    const int most_slept_n = *std::max_element(least_woke.sleeps, least_woke.sleeps+60);
    int most_aslept_minute = (std::find(least_woke.sleeps, least_woke.sleeps + 60, most_slept_n) - least_woke.sleeps);
    return least_woke.id * most_aslept_minute;
}

//------------------------------------------------------------------------------
#define TEST_EQ(a, b) { std::cout << #a << " == " << #b << " == " << ((a) == (b)) << std::endl; assert((a) == (b)); }

void test() {
    Log testL = parse_log("[1518-11-01 00:09] falls asleep");
    TEST_EQ(testL, (Log{1518, 11, 1, 0, 9, Log::FALL_ASLEEP, 0}));

    std::vector<Log> logs;
    logs.emplace_back(parse_log("[1518-11-01 00:05] falls asleep"));
    logs.emplace_back(parse_log("[1518-11-01 00:30] falls asleep"));
    logs.emplace_back(parse_log("[1518-11-01 00:00] Guard #10 begins shift"));
    logs.emplace_back(parse_log("[1518-11-02 00:50] wakes up"));
    logs.emplace_back(parse_log("[1518-11-01 00:25] wakes up"));
    logs.emplace_back(parse_log("[1518-11-01 00:55] wakes up"));
    logs.emplace_back(parse_log("[1518-11-05 00:45] falls asleep"));
    logs.emplace_back(parse_log("[1518-11-01 23:58] Guard #99 begins shift"));
    logs.emplace_back(parse_log("[1518-11-02 00:40] falls asleep"));
    logs.emplace_back(parse_log("[1518-11-03 00:24] falls asleep"));
    logs.emplace_back(parse_log("[1518-11-05 00:55] wakes up"));
    logs.emplace_back(parse_log("[1518-11-04 00:02] Guard #99 begins shift"));
    logs.emplace_back(parse_log("[1518-11-05 00:03] Guard #99 begins shift"));
    logs.emplace_back(parse_log("[1518-11-04 00:36] falls asleep"));
    logs.emplace_back(parse_log("[1518-11-04 00:46] wakes up"));
    logs.emplace_back(parse_log("[1518-11-03 00:05] Guard #10 begins shift"));
    logs.emplace_back(parse_log("[1518-11-03 00:29] wakes up"));

    sort(begin(logs), end(logs));
    assign_ids(logs);

    TEST_EQ(logs[0], (Log{1518, 11, 1, 0, 0, Log::BEGIN, 10}));
    TEST_EQ(logs[1], (Log{1518, 11, 1, 0, 5, Log::FALL_ASLEEP, 10}));
    TEST_EQ(logs[2], (Log{1518, 11, 1, 0, 25, Log::WAKE_UP, 10}));
    TEST_EQ(logs[5], (Log{1518, 11, 1, 23, 58, Log::BEGIN, 99}));
    TEST_EQ(logs[6], (Log{1518, 11, 2, 0, 40, Log::FALL_ASLEEP, 99}));

    TEST_EQ(minute_with_strat_1(logs), 240);
}

void run() {
    using namespace std::chrono;
    high_resolution_clock::time_point t_start = high_resolution_clock::now();

    std::vector<Log> logs;
    std::string line;
    while (getline(std::cin, line)) {
        logs.emplace_back(parse_log(line));
    }
    high_resolution_clock::time_point t_io = high_resolution_clock::now();

    sort(begin(logs), end(logs));
    assign_ids(logs);
    const auto answer = minute_with_strat_1(std::move(logs));

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

