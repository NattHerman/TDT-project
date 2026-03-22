#pragma once

#include "vec2.h"

namespace Hex {

// Axis-aligned rectangle
template <typename T>
struct rect {
    vec2<T> position{0, 0};
    vec2<T> size{0, 0};

    vec2<T> getCenter() {
        return position + size / 2;
    }

    rect() = default;
    rect(vec2<T> size): size{size} {}
    rect(vec2<T> position, vec2<T> size): position{position}, size{size} {}
};

} // namespace Hex
