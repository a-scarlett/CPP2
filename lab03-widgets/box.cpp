#include "box.hpp"

namespace widgets {

widget *box::child_at(int x, int y) {
    int current = 0;
    int i = 0;
    if (m_kind == box::kind::HORIZONTAL) {
        do {
            current += m_box[i]->width();
            i++;
        } while (i < static_cast<int>(m_box.size()) && current <= x);
        i--;
        current -= m_box[i]->width();
        if (y <= static_cast<int>(std::floor(
                     static_cast<float>((m_height + m_box[i]->height()) / 2.0)
                 )) &&
            y >= static_cast<int>(std::floor(
                     static_cast<float>((m_height - m_box[i]->height()) / 2.0)
                 ))) {
            if (dynamic_cast<container *>(m_box[i].get()) != nullptr) {
                return m_box[i]->child_at(
                    x - current,
                    y - static_cast<int>(std::ceil(static_cast<float>(
                            (m_height - m_box[i]->height()) / 2.0
                        )))
                );
            } else {
                return m_box[i].get();
            }
        } else {
            return nullptr;
        }
    } else {
        do {
            current += m_box[i]->height();
            i++;
        } while (i < static_cast<int>(m_box.size()) && current <= y);
        i--;
        current -= m_box[i]->height();
        if (x <= static_cast<int>(std::floor(
                     static_cast<float>((m_width + m_box[i]->width()) / 2.0)
                 )) &&
            x >= static_cast<int>(std::ceil(
                     static_cast<float>((m_width - m_box[i]->width()) / 2.0)
                 ))) {
            if (dynamic_cast<container *>(m_box[i].get()) != nullptr) {
                return m_box[i]->child_at(
                    x - static_cast<int>(std::ceil(static_cast<float>(
                            (m_width - m_box[i]->width()) / 2.0
                        ))),
                    y - current
                );
            } else {
                return m_box[i].get();
            }
        } else {
            return nullptr;
        }
    }
}

void box::update_layout() {
    m_width = 0;
    m_height = 0;
    if (m_kind == box::kind::HORIZONTAL) {
        for (auto &t : m_box) {
            m_height = std::max(t->height(), m_height);
            m_width += t->width();
        }
    } else {
        for (auto &t : m_box) {
            m_width = std::max(t->width(), m_width);
            m_height += t->height();
        }
    }
}

widget *box::add(std::unique_ptr<widget> child) {
    if (m_kind == box::kind::HORIZONTAL) {
        m_height = std::max(child->height(), m_height);
        m_width += child->width();
    } else {
        m_width = std::max(child->width(), m_width);
        m_height += child->height();
    }
    m_box.push_back(std::move(child));
    set_parent(m_box.back().get());
    return m_box.back().get();
}

std::unique_ptr<widget> box::remove(int index) {
    auto x = std::move(m_box[index]);
    m_box.erase(m_box.begin() + index);
    update_layout();
    remove_parent(x.get());
    return x;
}

std::unique_ptr<box> make_box(box::kind kind_) {
    return std::make_unique<box>(kind_);
}
}  // namespace widgets
