#ifndef GRID_HPP_
#define GRID_HPP_

#include <algorithm>
#include <cmath>
#include <memory>
#include <typeinfo>
#include <utility>
#include <vector>
#include "abstract_widgets.hpp"

namespace widgets {
struct grid final : container {
    [[nodiscard]] int width() const final {
        return m_width;
    };

    [[nodiscard]] int height() const final {
        return m_height;
    };

    explicit grid(int x, int y);

    [[nodiscard]] int rows() const {
        return static_cast<int>(heights.size());
    };

    [[nodiscard]] int columns() const {
        return widths.empty() ? 0 : static_cast<int>(widths.size());
    };

    void update_layout() final;

    widget *child_at(int x, int y) final;

    [[nodiscard]] widget *get(int x, int y) const {
        return (m_grid[x][y] == nullptr) ? nullptr : m_grid[x][y].get();
    };

    widget *add(std::unique_ptr<widget> child, int row, int column);

    std::unique_ptr<widget> remove(int row, int column);

private:
    std::vector<std::vector<std::unique_ptr<widget>>> m_grid;
    std::vector<int> heights;
    std::vector<int> widths;
};

std::unique_ptr<grid> make_grid(int x, int y);
}  // namespace widgets

#endif  // GRID_HPP_
