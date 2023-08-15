#include <boost/dll/import.hpp>
#include <boost/version.hpp>
#include <iostream>
#include <memory>
#include <typeinfo>
#include "view.hpp"

#if BOOST_VERSION >= 107600
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define boost_dll_import_symbol ::boost::dll::import_symbol
#else
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define boost_dll_import_symbol ::boost::dll::import
#endif

int main(  // NOLINT(bugprone-exception-escape)
    [[maybe_unused]] int argc,
    char **argv
) {  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    std::string lib(argv[1]);
    // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
    boost::shared_ptr<tictactoe::View> view =
        boost_dll_import_symbol<tictactoe::View>(
            lib, "view", boost::dll::load_mode::append_decorations
        );
    // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
    tictactoe::Game g;

    std::cout << std::endl;
    view->display(g);

    std::string string;
    std::pair<int, int> input;
    bool finish = false;
    while (!finish) {
        g.invitation();
        std::getline(std::cin, string);
        if (string.empty()) {
            break;
        }
        // cppcheck-suppress stlIfStrFind
        if (!string.empty() && string.find("view-") == 0) {
            view = boost_dll_import_symbol<tictactoe::View>(
                string, "view", boost::dll::load_mode::append_decorations
            );
            view->display(g);
        } else {
            input = view->read(g, string, finish);
            if (finish) {
                break;
            } else {
                if (g.move(input)) {
                    std::cout << std::endl;
                    view->display(g);
                }
            }
        }
        finish = g.is_finished();
    }
}
