#ifndef TTT_COMPLEX_VIEW_H
#define TTT_COMPLEX_VIEW_H

#include "view.hpp"

namespace tictactoe {

struct compact_view : View {
    void display(const Game &) override;
    std::pair<int, int> read(const Game &, std::string &string, bool &finish)
        override;
};

}  // namespace tictactoe

#endif  // TTT_COMPLEX_VIEW_H
