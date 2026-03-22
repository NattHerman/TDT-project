#pragma once

#include "vec2.h"

namespace Hex {

// Axis-aligned rectangle, position is top left corner.
template <typename T>
struct rect {
    vec2<T> position{0, 0};
    // DO NOT set size to negative values.
    vec2<T> size{0, 0};

    vec2<T> getCenter() {
        return position + size / 2;
    }

    // Expands to fit self and other rectangle
    void engulf(rect<T> other);

    rect() = default;
    rect(vec2<T> size): size{size} {}
    rect(vec2<T> position, vec2<T> size): position{position}, size{size} {}
};


template <typename T>
void rect<T>::engulf(rect<T> other)  {
    vec2<T> topLeft = std::min(position, other.position);
    vec2<T> bottomRight = std::max(position + size, other.position + other.size);

    position = topLeft;
    size = bottomRight - topLeft;
}

template <typename T>
rect<T> operator+(const rect<T> &lhs, const vec2<T> &rhs) {
    return rect<T>{lhs.position + rhs, lhs.size};
}

template <typename T>
std::ostream& operator<<(std::ostream &lhs, const rect<T> &rhs) {
    lhs << "[" << rhs.position << " " << rhs.size << "]";
    return lhs;
}

} // namespace Hex
