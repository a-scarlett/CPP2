#ifndef LAB04_TIC_TAC_TOE_ABSTRACT_VIEW_H
#define LAB04_TIC_TAC_TOE_ABSTRACT_VIEW_H
#include <boost/config.hpp>
#include <string>
#include "game.hpp"

namespace tictactoe {

struct BOOST_SYMBOL_VISIBLE View {
    virtual ~View() = default;
    View() = default;
    View(const View &other) = delete;
    View &operator=(const View &other) = delete;
    View(View &&other) noexcept = delete;
    View &operator=(View &&other) noexcept = default;

    virtual std::pair<int, int>
    read(const Game &, std::string &string, bool &finish) = 0;
    virtual void display(const Game &) = 0;
};

}  // namespace tictactoe

#endif  // LAB04_TIC_TAC_TOE_ABSTRACT_VIEW_H
