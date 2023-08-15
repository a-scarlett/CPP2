#include "ball_icon.hpp"

namespace widgets {
[[nodiscard]] int ball_icon::width() const {
    return 2 * m_radius + 1;
}

[[nodiscard]] int ball_icon::height() const {
    return 2 * m_radius + 1;
}

widget *ball_icon::child_at(int x, int y) {
    if ((x - m_radius) * (x - m_radius) + (y - m_radius) * (y - m_radius) <=
        m_radius * m_radius) {
        return this;
    } else {
        return nullptr;
    }
}

ball_icon::ball_icon(int radius) : m_radius(radius) {
}

[[nodiscard]] std::unique_ptr<ball_icon> make_ball_icon(int radius) {
    return std::make_unique<ball_icon>(radius);
}

}  // namespace widgets
