#ifndef TTT_GAME_H
#define TTT_GAME_H

#include <iostream>
#include <vector>

namespace tictactoe {
class Game {
public:
    static const int SIZE = 10;
    static const int Count_for_win = 5;

    enum class OptionalPlayer { EMPTY = 0, X = 2, O = 1 };

    Game();
    [[nodiscard]] bool is_winner(OptionalPlayer player);

    void invitation() const;

    [[nodiscard]] OptionalPlayer get_cell(std::pair<int, int> input) const;

    [[nodiscard]] bool move(std::pair<int, int> input);

    [[nodiscard]] bool is_finished();

private:
    std::vector<std::vector<OptionalPlayer>> field;
    unsigned number_of_moves = 0;
};

}  // namespace tictactoe

#endif  // TTT_GAME_H
