#ifndef LAB04_TIC_TAC_TOE_SIMPLE_VIEW_H
#define LAB04_TIC_TAC_TOE_SIMPLE_VIEW_H

#include "view.hpp"

namespace tictactoe {

struct pretty_view : View {
    void display(const Game &) override;
    std::pair<int, int> read(const Game &, std::string &string, bool &finish)
        override;
    static void horizontal();
    static void alphabet();
};

}  // namespace tictactoe

#endif  // LAB04_TIC_TAC_TOE_SIMPLE_VIEW_H
