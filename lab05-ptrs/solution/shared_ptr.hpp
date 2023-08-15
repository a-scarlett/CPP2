#pragma once
#include <iostream>

namespace ptrs::shared {

template <typename T>
class shared_ptr {
public:
    shared_ptr() noexcept : m_ptr(nullptr), m_count(nullptr) {
    }

    explicit shared_ptr(std::nullptr_t) noexcept
        : m_ptr(nullptr), m_count(nullptr) {
    }

    explicit shared_ptr(T *ptr) : m_ptr(ptr), m_count(new int(1)) {
    }

    shared_ptr(const shared_ptr &&other) noexcept
        : m_ptr(other.m_ptr), m_count(other.m_count) {
        if (m_count != nullptr) {
            ++(*m_count);
        }
    }

    shared_ptr(shared_ptr &&other) noexcept
        : m_ptr(other.m_ptr), m_count(other.m_count) {
        other.m_count = nullptr;
        other.m_ptr = nullptr;
    }

    shared_ptr &operator=(shared_ptr &&other) noexcept {
        if (this != &other) {
            reset();
            m_count = other.m_count;
            m_ptr = other.m_ptr;
            other.m_count = nullptr;
            other.m_ptr = nullptr;
        }
        return *this;
    }

    shared_ptr(const shared_ptr &other) noexcept
        : m_ptr(other.m_ptr), m_count(other.m_count) {
        if (m_count != nullptr) {
            ++(*m_count);
        }
    }

    shared_ptr &operator=(const shared_ptr &other) noexcept {
        if (this != &other) {
            release();
            m_ptr = other.m_ptr;
            m_count = other.m_count;
            if (m_count != nullptr) {
                ++(*m_count);
            }
        }
        return *this;
    }

    ~shared_ptr() {
        release();
    }

    bool operator==(const shared_ptr &other) const noexcept {
        return m_ptr == other.m_ptr;
    }

    bool operator!=(const shared_ptr &other) const noexcept {
        return m_ptr != other.m_ptr;
    }

    T *get() const noexcept {
        return m_ptr;
    }

    T &operator*() const noexcept {
        return *m_ptr;
    }

    T *operator->() const noexcept {
        return m_ptr;
    }

    explicit operator bool() const noexcept {
        return m_ptr != nullptr;
    }

    void swap(shared_ptr &other) noexcept {
        std::swap(m_ptr, other.m_ptr);
        std::swap(m_count, other.m_count);
    }

    friend void swap(shared_ptr<T> &a, shared_ptr<T> &b) noexcept {
        a.swap(b);
    }

    void reset() noexcept {
        shared_ptr().swap(*this);
    }

    template <typename Y>
    void reset(Y *pointer) {
        shared_ptr(pointer).swap(*this);
    }

private:
    void release() {
        if (m_count != nullptr) {
            --(*m_count);
            if (*m_count == 0) {
                delete m_ptr;
                delete m_count;
            }
            m_ptr = nullptr;
            m_count = nullptr;
        }
    }

    T *m_ptr;
    int *m_count;
};

}  // namespace ptrs::shared