#include "grid.hpp"

namespace widgets {

void grid::update_layout() {
    m_width = 0;
    m_height = 0;

    for (int row = 0; row < rows(); row++) {
        auto max_height = std::max_element(
            m_grid[row].begin(), m_grid[row].end(),
            [](const std::unique_ptr<widget> &a,
               const std::unique_ptr<widget> &b) {
                return (
                    a == nullptr || (b != nullptr && a->height() < b->height())
                );
            }
        );
        if (*max_height == nullptr) {
            heights[row] = 0;
        } else {
            heights[row] = (*max_height)->height();
            m_height += heights[row];
        }
    }

    for (int column = 0; column < columns(); column++) {
        auto max_width = std::max_element(
            m_grid.begin(), m_grid.end(),
            [column](const auto &a, const auto &b) {
                return (
                    a[column] == nullptr ||
                    (b[column] != nullptr &&
                     a[column]->width() < b[column]->width())
                );
            }
        );
        if ((*max_width)[column] == nullptr) {
            widths[column] = 0;
        } else {
            widths[column] = (*max_width)[column]->width();
            m_width += widths[column];
        }
    }
}

widget *grid::child_at(int x, int y) {
    if (m_grid.empty()) {
        return nullptr;
    }

    int current_x = widths[0];
    int current_y = heights[0];
    int i = 0;
    int j = 0;

    for (; i < columns() - 1 && current_x <= x; i++) {
        current_x += widths[i + 1];
    }
    i = std::min(i, columns() - 1);

    for (; j < rows() - 1 && current_y <= y; j++) {
        current_y += heights[j + 1];
    }
    j = std::min(j, rows() - 1);

    if (m_grid[j][i] != nullptr &&
        x < current_x - widths[i] + m_grid[j][i]->width() &&
        y < current_y - heights[j] + m_grid[j][i]->height()) {
        if (dynamic_cast<container *>(m_grid[j][i].get()) != nullptr) {
            current_x -= widths[i];
            current_y -= heights[j];
            return m_grid[i][j]->child_at(x - current_x, y - current_y);
        } else {
            return m_grid[j][i].get();
        }
    } else {
        return nullptr;
    }
}

widget *grid::add(std::unique_ptr<widget> child, int row, int column) {
    set_parent(child.get());
    if (child->height() > heights[row]) {
        m_height += child->height() - heights[row];
        heights[row] = child->height();
    }

    if (child->width() > widths[column]) {
        m_width += child->width() - widths[column];
        widths[column] = child->width();
    }

    m_grid[row][column] = std::move(child);
    return m_grid[row][column].get();
}

std::unique_ptr<widget> grid::remove(int row, int column) {
    auto x = std::move(m_grid[row][column]);

    auto max_height = std::max_element(
        m_grid[row].begin(), m_grid[row].end(),
        [](const std::unique_ptr<widget> &a, const std::unique_ptr<widget> &b) {
            return (
                a == nullptr || (b != nullptr && a->height() < b->height())
            );
        }
    );

    if (*max_height == nullptr) {
        m_height -= heights[row];
        heights[row] = 0;
    } else {
        m_height -= heights[row];
        heights[row] = (*max_height)->height();
        m_height += heights[row];
    }

    auto max_width = std::max_element(
        m_grid.begin(), m_grid.end(),
        [column](const auto &a, const auto &b) {
            return (
                a[column] == nullptr ||
                (b[column] != nullptr && a[column]->width() < b[column]->width()
                )
            );
        }
    );
    if ((*max_width)[column] == nullptr) {
        m_width -= widths[column];
        widths[column] = 0;
    } else {
        m_width -= widths[column];
        widths[column] = (*max_width).at(column)->width();
        m_width += widths[column];
    }
    remove_parent(x.get());
    return x;
}

grid::grid(int x, int y) {
    m_grid.resize(x);
    widths.resize(y, 0);
    heights.resize(x, 0);
    for (auto &t : m_grid) {
        t.resize(y);
    }
}

std::unique_ptr<grid> make_grid(int x, int y) {
    return std::make_unique<grid>(x, y);
}
}  // namespace widgets