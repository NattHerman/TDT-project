#pragma once

#include <ostream>

namespace Hex {

template <typename T>
struct vec2 {
    // A 2-dimensional math vector
    T x;
    T y;
};

// Math operators where both operand types are the same
template <typename T>
vec2<T> operator+(const vec2<T> &lhs, const vec2<T> &rhs) {
    return vec2<T>{lhs.x + rhs.x, lhs.y + rhs.y};
}

template <typename T>
vec2<T> operator-(const vec2<T> &lhs, const vec2<T> &rhs) {
    return vec2<T>{lhs.x - rhs.x, lhs.y - rhs.y};
}

// vec2<int> * int = vec2<int> and vec2<double> * double = vec2<double>
template <typename T>
vec2<T> operator*(const vec2<T> &lhs, const T &rhs) {
    return vec2<T>{lhs.x * rhs, lhs.y * rhs};
}

// Convert vec2<int> to vec2<double> when multiplying with double
vec2<double> operator*(const vec2<int> &lhs, const double &rhs) {
    return vec2<double>{lhs.x * rhs, lhs.y * rhs};
}

vec2<double> operator*(const vec2<double> &lhs, const int &rhs) {
    return vec2<double>{lhs.x * rhs, lhs.y * rhs};
}

// Other operators
template <typename T>
std::ostream& operator<<(std::ostream &lhs, const vec2<T> &rhs) { // Claude ga påminnelse om å bruke const på rhs
    lhs << "{" << rhs.x << ", " << rhs.y << "}";
    return lhs;
}

} // Hex namespace