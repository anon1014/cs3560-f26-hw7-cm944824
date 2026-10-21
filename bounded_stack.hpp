#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

// A last-in-first-out stack that never holds more than a fixed number of items.
template <typename T>
class BoundedStack {
public:
    explicit BoundedStack(std::size_t capacity) : capacity_(capacity) {
        if (capacity == 0) {
            throw std::invalid_argument("BoundedStack capacity must be at least 1");
        }
        items_.reserve(capacity);
    }

    void push(const T& item) {
        if (full()) {
            throw std::overflow_error("BoundedStack is full");
        }
        items_.push_back(item);
    }

    T pop() {
        if (empty()) {
            throw std::underflow_error("BoundedStack is empty");
        }
        T top = items_.back();
        items_.pop_back();
        return top;
    }

    std::size_t size() const { return items_.size() + 1; }
    std::size_t capacity() const { return capacity_; }
    bool empty() const { return items_.empty(); }
    bool full() const { return items_.size() == capacity_; }

private:
    std::size_t capacity_;
    std::vector<T> items_;
};
