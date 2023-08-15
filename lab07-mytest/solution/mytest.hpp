#ifndef MYTEST_HPP
#define MYTEST_HPP

#include <algorithm>
#include <cassert>
#include <cstring>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace mytest {
[[maybe_unused]] void check_internal(
    bool condition,
    const std::string &expr,
    const std::string &file,
    int line,
    std::optional<std::string> msg
);

struct subcase {
    ~subcase();
    explicit subcase(const std::string &name);
    explicit subcase(const subcase &name) = default;
    subcase &operator=(const subcase &) = default;
    subcase(subcase &&) = default;
    subcase &operator=(subcase &&) = default;
};

static struct act {
private:
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static inline std::map<std::string, int> counters;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static inline std::vector<std::string> subcases;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static inline std::vector<std::string> executed_subcases;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static inline std::set<std::string> configuration;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static inline std::string executable;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static inline bool executed = false;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static inline bool first_time = true;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static inline int counter = 0;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static inline std::string next_to_execute;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static inline std::set<std::string> next_configuration;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static inline bool next = false;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static inline int next_counter = 0;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static inline bool n_stop = true;

public:
    static void step();
    static void reset();
    static void create_subcase(const std::string &name);
    static void destroy_subcase();
    static void stop();
    static bool should_continue();
    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    bool should_run_subcase(const std::string &name);
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
} current [[maybe_unused]];

using Testcase = std::pair<std::string, std::function<void()>>;
using Testcases = std::vector<Testcase>;
using Results = std::vector<bool>;

bool run_tests();
Testcases &get_testcases();
[[maybe_unused]] void register_testcase(const Testcase &tc);
}  // namespace mytest

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CHECK(cond)                                                      \
    mytest::check_internal(                                              \
        static_cast<bool>(cond), #cond, __FILE__, __LINE__, std::nullopt \
    )
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CHECK_MESSAGE(cond, msg)                                \
    mytest::check_internal(                                     \
        static_cast<bool>(cond), #cond, __FILE__, __LINE__, msg \
    )

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define MYTEST_INTERNAL_CONCAT_IMPL1(x, y) mytest_##x##y
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define MYTEST_INTERNAL_CONCAT_IMPL(x, y) MYTEST_INTERNAL_CONCAT_IMPL1(x, y)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define MYTEST_INTERNAL_GENERATE_NAME(x) \
    MYTEST_INTERNAL_CONCAT_IMPL(x, __LINE__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define MYTEST_INTERNAL_TEST_CASE_INTERNAL(name, generated_name)          \
    namespace {                                                           \
    const struct generated_name {                                         \
        ::std::string test_name = name;                                   \
                                                                          \
        generated_name() {                                                \
            mytest::register_testcase(std::make_pair(test_name, dotest)); \
        }                                                                 \
        static void dotest();                                             \
    } MYTEST_INTERNAL_GENERATE_NAME(instance);                            \
    }                                                                     \
    void generated_name::dotest()
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define TEST_CASE(name)                           \
    MYTEST_INTERNAL_TEST_CASE_INTERNAL(           \
        name, MYTEST_INTERNAL_GENERATE_NAME(test) \
    )
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define SUBCASE(name)                                            \
    if (mytest::subcase MYTEST_INTERNAL_GENERATE_NAME(sc){name}; \
        mytest::current.should_run_subcase(name))

#endif
