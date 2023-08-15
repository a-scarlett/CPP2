#include "view_pretty.hpp"
#include <iostream>
#include <map>

const static std::map<tictactoe::Game::OptionalPlayer, std::pair<char, char> >
    pretty_cell{
        {tictactoe::Game::OptionalPlayer::EMPTY, {' ', ' '}},
        {tictactoe::Game::OptionalPlayer::X, {'\\', '/'}},
        {tictactoe::Game::OptionalPlayer::O, {'@', '@'}},
    };

namespace tictactoe {

std::pair<int, int> pretty_view::read(
    const Game &,
    std::string &string,
    [[maybe_unused]] bool &finish
) {
    int x = 0;
    int y = 0;
    if (string.size() != 2) {
        x = -2;
        y = -2;
    } else {
        y = static_cast<int>(string[0]) - static_cast<int>('a');
        x = static_cast<int>('9') - static_cast<int>(string[1]);
    }
    return {x, y};
}

void pretty_view::alphabet() {
    for (int i = 0; i < tictactoe::Game::SIZE; i++) {
        std::cout << "    " << static_cast<char>('a' + i);
    }
    std::cout << "   " << std::endl;
}

void pretty_view::horizontal() {
    std::cout << "  ";
    for (int i = 0; i < tictactoe::Game::SIZE; i++) {
        std::cout << "+";
        for (int j = 0; j < 4; j++) {
            std::cout << "-";
        }
    }
    std::cout << "+";
    std::cout << std::endl;
}

void pretty_view::display(const Game &g) {
    for (int i = 0; i < tictactoe::Game::SIZE; i++) {
        horizontal();
        std::cout << tictactoe::Game::SIZE - 1 - i << ' ' << '|';
        for (int j = 0; j < tictactoe::Game::SIZE; j++) {
            std::cout << ' ' << pretty_cell.at(g.get_cell({i, j})).first
                      << pretty_cell.at(g.get_cell({i, j})).second << ' '
                      << '|';
        }
        std::cout << std::endl;
        std::cout << ' ' << ' ' << '|';
        for (int j = 0; j < tictactoe::Game::SIZE; j++) {
            std::cout << ' ' << pretty_cell.at(g.get_cell({i, j})).second
                      << pretty_cell.at(g.get_cell({i, j})).first << ' ' << '|';
        }
        std::cout << std::endl;
    }
    horizontal();
    alphabet();
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
extern "C" BOOST_SYMBOL_EXPORT pretty_view view;
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
pretty_view view;

}  // namespace tictactoe
