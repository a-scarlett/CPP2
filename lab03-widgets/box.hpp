#ifndef BOX_HPP_
#define BOX_HPP_

#include <cmath>
#include <memory>
#include <typeinfo>
#include <utility>
#include <vector>
#include "abstract_widgets.hpp"

namespace widgets {
struct box final : container {
    [[nodiscard]] int width() const final {
        return m_width;
    };

    [[nodiscard]] int height() const final {
        return m_height;
    };

    enum kind { HORIZONTAL, VERTICAL };

    explicit box(box::kind kind_) : m_kind(kind_) {
    }

    void update_layout() final;

    widget *child_at(int x, int y) final;

    [[nodiscard]] int size() const {
        return static_cast<int>(m_box.size());
    }

    [[nodiscard]] widget *get(int index) const {
        return m_box[index].get();
    };

    widget *add(std::unique_ptr<widget> child);

    std::unique_ptr<widget> remove(int index);

private:
    const kind m_kind;
    std::vector<std::unique_ptr<widget>> m_box;
};

std::unique_ptr<box> make_box(box::kind kind_);

}  // namespace widgets

#endif  // BOX_HPP_
