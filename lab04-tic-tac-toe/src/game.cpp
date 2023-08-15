#include "game.hpp"
#include <algorithm>
#include <iostream>
#include <random>

namespace tictactoe {

Game::Game()
    : field(
          SIZE,
          std::vector<OptionalPlayer>(SIZE, Game::OptionalPlayer::EMPTY)
      ) {
}

[[nodiscard]] bool Game::move([[maybe_unused]] std::pair<int, int> input) {
    if (input.first > SIZE - 1 || input.first < 0 || input.second > SIZE - 1 ||
        input.second < 0) {
        std::cout << "Bad move!" << std::endl;
        return false;
    }
    if (field[input.first][input.second] != OptionalPlayer::EMPTY) {
        std::cout << "Bad move!" << std::endl;
        return false;
    }
    field[input.first][input.second] =
        static_cast<tictactoe::Game::OptionalPlayer>(number_of_moves % 2 + 1);
    number_of_moves++;
    return true;
}

[[nodiscard]] Game::OptionalPlayer Game::get_cell(std::pair<int, int> input
) const {
    return field[input.first][input.second];
}

void Game::invitation() const {
    if (number_of_moves % 2 == 0) {
        std::cout << "O move: ";
    } else {
        std::cout << "X move: ";
    }
}

[[nodiscard]] bool
Game::is_finished(  // NOLINT(readability-function-cognitive-complexity)
) {
    if (number_of_moves == SIZE * SIZE) {
        std::cout << "Draw." << std::endl;
        return true;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (field[i][j] == OptionalPlayer::EMPTY) {
                continue;
            } else {
                std::vector<bool> win(4, true);
                for (int t = 0; t < Count_for_win; t++) {
                    if (t + j >= SIZE || field[i][j] != field[i][t + j]) {
                        win[0] = false;
                    }
                    if (t + i >= SIZE || field[i][j] != field[t + i][j]) {
                        win[1] = false;
                    }
                    if (t + i >= SIZE || j + t >= SIZE ||
                        field[i][j] != field[i + t][j + t]) {
                        win[2] = false;
                    }
                    if (i - t < 0 || j + t >= SIZE ||
                        field[i][j] != field[i - t][j + t]) {
                        win[3] = false;
                    }
                }
                if (std::any_of(win.begin(), win.end(), [](bool x) {
                        return x;
                    })) {
                    if (field[i][j] == OptionalPlayer::O) {
                        std::cout << "O wins!" << std::endl;
                    } else {
                        std::cout << "X wins!" << std::endl;
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

}  // namespace tictactoe
