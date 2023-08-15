#ifndef ABSTRACT_WIDGETS_HPP_
#define ABSTRACT_WIDGETS_HPP_

#include <memory>
#include <typeinfo>

namespace widgets {
struct container;

struct widget {
    [[nodiscard]] virtual int width() const = 0;
    [[nodiscard]] virtual int height() const = 0;

    widget(const widget &) = delete;
    widget(widget &&) = delete;
    widget &operator=(const widget &) = delete;
    widget &operator=(widget &&) = delete;
    virtual widget *child_at(int x, int y) = 0;
    virtual ~widget() = default;

    [[nodiscard]] container *parent() const {
        return m_parent;
    };

protected:
    widget() = default;

private:
    friend struct container;
    container *m_parent = nullptr;
};

struct container : widget {
    virtual void update_layout() = 0;

protected:
    // NOLINTNEXTLINE(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes)
    int m_width = 0;
    // NOLINTNEXTLINE(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes)
    int m_height = 0;

    void set_parent(widget *child) {
        child->m_parent = this;
    }

    void static remove_parent(widget *child) {
        child->m_parent = nullptr;
    }
};
}  // namespace widgets

#endif  // ABSTRACT_WIDGETS_HPP_
