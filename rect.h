#pragma once

#include "vec2.h"

// Axis-aligned rectangle
namespace Hex {

template <typename T>
struct rect {
    vec2<T> position{0, 0};
    vec2<T> size{0, 0};
};

} // namespace Hex
