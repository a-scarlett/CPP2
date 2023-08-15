#include "mytest.hpp"

namespace mytest {

void act::step() {
    executed_subcases.clear();
    executed = false;
    first_time = false;
    next = false;
    executable = next_to_execute;
    counter = next_counter;
    configuration.clear();
    configuration.insert(next_configuration.begin(), next_configuration.end());
    next_configuration.clear();
    counters.clear();
}

void act::reset() {
    counters.clear();
    subcases.clear();
    executed_subcases.clear();
    configuration.clear();
    executable = "";
    executed = false;
    first_time = true;
    counter = 0;
    next_to_execute = "";
    next_configuration.clear();
    next = false;
    next_counter = 0;
    n_stop = true;
}

void act::create_subcase(const std::string &name) {
    counters[name]++;
    if (current.should_run_subcase(name)) {
        executed_subcases.push_back(name);
    }
    if (executed) {
        if (!next) {
            next_configuration.insert(subcases.begin(), subcases.end());
            next_to_execute = name;
            next_counter = counters[name];
            next = true;
        }
    }
    if (name == executable && counter == counters[name]) {
        first_time = true;
    }
    subcases.push_back(name);
}

void act::destroy_subcase() {
    if (first_time && !executed) {
        executed = true;
        first_time = false;
    }
    subcases.pop_back();
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
bool act::should_run_subcase(const std::string &name) {
    return (
        !executed &&
        (first_time || configuration.find(name) != configuration.end() ||
         (name == executable && counter == counters[name]))
    );
}

void act::stop() {
    for (const auto &t : executed_subcases) {
        std::cerr << "    in subcase " << t << std::endl;
    }
    n_stop = false;
}

bool act::should_continue() {
    return n_stop && next && executed;
}

Testcases &get_testcases() {
    static Testcases testcases;
    return testcases;
}

Results &get_results() {
    static Results results;
    return results;
}

[[maybe_unused]] void register_results(bool res) {
    get_results().push_back(res);
}

// cppcheck-suppress unusedFunction
[[maybe_unused]] void register_testcase(const Testcase &tc) {
    get_testcases().push_back(tc);
}

[[maybe_unused]] void check_internal(  // cppcheck-suppress unusedFunction
    bool condition,
    const std::string &expr,
    const std::string &file,
    int line,
    std::optional<std::string> msg
) {
    bool res = condition;
    if (!res) {
        std::cerr << "CHECK(" << expr << ") at " << file << ":" << line
                  << " failed!" << std::endl;
        if (msg.has_value()) {
            std::cerr << "    message: " << msg.value() << std::endl;
        }
        current.stop();
    }
    register_results(res);
}

bool sortbyname(
    const std::pair<std::string, std::function<void()>> &a,
    const std::pair<std::string, std::function<void()>> &b
) {
    return a.first < b.first;
}

bool run_tests [[maybe_unused]] () {
    int count = 0;
    int all = 0;
    std::sort(get_testcases().begin(), get_testcases().end(), sortbyname);
    for (auto &tc : get_testcases()) {
        bool passed = true;
        std::cerr << "Running \"" << tc.first << "\"..." << std::endl;
        tc.second();
        while (current.should_continue()) {
            current.step();
            std::cerr << "...another subcase..." << std::endl;
            tc.second();
        }
        if (std::any_of(get_results().begin(), get_results().end(), [](bool i) {
                return !i;
            })) {
            passed = false;
        }
        count += static_cast<int>(passed);
        all++;
        get_results().clear();
        current.reset();
    }
    std::cerr << "===== Tests passed: " << count << "/" << all
              << " =====" << std::endl;
    return (count != all);
}

subcase::subcase(const std::string &name) {
    current.create_subcase(name);
}

subcase::~subcase() {
    current.destroy_subcase();
}
}  // namespace mytest
