#include "view_compact.hpp"
#include <iostream>
#include <sstream>

namespace tictactoe {

std::pair<int, int>
compact_view::read(const Game &, std::string &string, bool &finish) {
    std::stringstream stream;
    stream << string;
    int x = 0;
    int y = 0;
    int check = 0;
    if (stream >> x >> y && !(stream >> check)) {
        if (x == -1 && y == -1) {
            finish = true;
        }
        return std::pair<int, int>{x, y};
    }
    return {-2, -2};
};

void compact_view::display(const Game &g) {
    for (int i = 0; i < Game::SIZE; i++) {
        for (int j = 0; j < Game::SIZE; j++) {
            switch (g.get_cell({i, j})) {
                case Game::OptionalPlayer::EMPTY:
                    std::cout << '.';
                    break;
                case Game::OptionalPlayer::X:
                    std::cout << 'X';
                    break;
                case Game::OptionalPlayer::O:
                    std::cout << 'O';
                    break;
            }
        }
        std::cout << std::endl;
    }
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
extern "C" BOOST_SYMBOL_EXPORT compact_view view;
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
compact_view view;
}  // namespace tictactoe
