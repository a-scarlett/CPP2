#pragma once
#include <iostream>
#include <type_traits>
#include <utility>

namespace ptrs::unique {

template <typename T>
struct default_delete {
    void operator()(T *ptr) const {
        delete ptr;
    }
};

template <typename T, typename D = default_delete<T>>
class unique_ptr {
public:
    unique_ptr() noexcept : ptr(nullptr), deleter(D()) {
    }

    explicit unique_ptr(std::nullptr_t) noexcept : ptr(nullptr), deleter(D()) {
    }

    explicit unique_ptr(T *p) noexcept : ptr(p), deleter(D()) {
    }

    explicit unique_ptr(T *p, D del) noexcept
        : ptr(p), deleter(std::move(del)) {
    }

    ~unique_ptr() {
        if (ptr != nullptr) {
            deleter(ptr);
        }
    }

    unique_ptr(const unique_ptr &) = delete;
    unique_ptr &operator=(const unique_ptr &) = delete;

    unique_ptr(unique_ptr &&other) noexcept
        : ptr(other.ptr), deleter(std::move(other.deleter)) {
        other.ptr = nullptr;
    }

    unique_ptr &operator=(unique_ptr &&other) noexcept {
        if (this != &other) {
            deleter(ptr);
            ptr = other.ptr;
            deleter = std::move(other.deleter);
            other.ptr = nullptr;
        }
        return *this;
    }

    bool operator==(const unique_ptr &other) const noexcept {
        return ptr == other.ptr;
    }

    bool operator!=(const unique_ptr &other) const noexcept {
        return ptr != other.ptr;
    }

    T *get() const noexcept {
        return ptr;
    }

    T &operator*() const noexcept {
        return *ptr;
    }

    T *operator->() const noexcept {
        return ptr;
    }

    explicit operator bool() const noexcept {
        return ptr != nullptr;
    }

    void reset(T *p = nullptr) noexcept {
        deleter(ptr);
        ptr = p;
    }

    T *release() noexcept {
        return std::exchange(ptr, nullptr);
    }

    void swap(unique_ptr &other) noexcept {
        std::swap(ptr, other.ptr);
        std::swap(deleter, other.deleter);
    }

    friend void swap(unique_ptr<T, D> &a, unique_ptr<T, D> &b) noexcept {
        a.swap(b);
    }

private:
    T *ptr;
    D deleter;
};

template <typename T, typename... Args>
unique_ptr<T> make_unique(Args &&...args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}  // namespace ptrs::unique