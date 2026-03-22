#pragma once

#include "vec2.h"

namespace Hex {

// Axis-aligned rectangle
template <typename T>
struct rect {
    vec2<T> position{0, 0};
    vec2<T> size{0, 0};
};

} // namespace Hex
