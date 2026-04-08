#pragma once

#include <ostream>
#include <string>

#include "Point.h"

namespace Hex {

template <typename T>
struct vec2 {
    // A 2-dimensional math vector
    T x;
    T y;

    // Cast from vec<T> to TDT4102::Point. From Claude
    operator TDT4102::Point() const {
        return TDT4102::Point{static_cast<int>(x), static_cast<int>(y)};
    }

    // Cast from vec<T> to vec<U>. Also with help from Claude
    template <typename U>
    explicit operator vec2<U>() const {
        return vec2<U>{static_cast<U>(x), static_cast<U>(y)};
    }

    double length_squared() {
        return x*x + y*y;
    }

    double length() {
        return sqrt(length_squared());
    }

    std::string toString() const {
        return "{" + std::to_string(x) + ", " + std::to_string(y) + "}";
    }

    // Constructors
    vec2() = default;
    vec2(T x, T y): x{x}, y{y} {};
    template<typename U>
    vec2(vec2<U> other): x{other.x}, y{other.y} {};
    // claude suggestion, construct from TDT4102::Point to vec2<T>
    vec2(const TDT4102::Point &point): x{point.x}, y{point.y} {} 
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

template <typename T>
bool operator!=(const vec2<T> &lhs, const vec2<T> &rhs) {
    return lhs.x != rhs.x || lhs.y != rhs.y;
}

template <typename T>
bool operator==(const vec2<T> &lhs, const vec2<T> &rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

// MULTIPLICATION

// vec2<int> * int = vec2<int> and vec2<double> * double = vec2<double>
template <typename T>
vec2<T> operator*(const vec2<T> &lhs, const T &rhs) {
    return vec2<T>{lhs.x * rhs, lhs.y * rhs};
}
template <typename T> // Mirror
vec2<T> operator*(const T &rhs, const vec2<T> &lhs) { return lhs * rhs; }

// Convert vec2<int> to vec2<double> when multiplying with double
inline vec2<double> operator*(const vec2<int> &lhs, const double &rhs) { // inline suggested by claude
    return vec2<double>{lhs.x * rhs, lhs.y * rhs};
}
// Mirror
inline vec2<double> operator*(const double &rhs, const vec2<int> &lhs) { return lhs * rhs; }

inline vec2<double> operator*(const vec2<double> &lhs, const int &rhs) { // inline suggested by claude
    return vec2<double>{lhs.x * rhs, lhs.y * rhs};
}
// Mirror
inline vec2<double> operator*(const int &rhs, const vec2<double> &lhs) { return lhs * rhs; }

// DIVISION

template <typename T>
vec2<T> operator/(const vec2<T> &lhs, const T &rhs) {
    return vec2<T>{lhs.x / rhs, lhs.y / rhs};
}

inline vec2<double> operator/(const vec2<int> &lhs, const double &rhs) {
    return vec2<double>{lhs.x / rhs, lhs.y / rhs};
}

inline vec2<double> operator/(const vec2<double> &lhs, const int &rhs) {
    return vec2<double>{lhs.x / rhs, lhs.y / rhs};
}

// Other operators
template <typename T>
std::ostream& operator<<(std::ostream &lhs, const vec2<T> &rhs) { // Claude ga påminnelse om å bruke const på rhs
    lhs << "{" << rhs.x << ", " << rhs.y << "}";
    return lhs;
}

} // Hex namespace

template <typename T>
Hex::vec2<T> std::min(const Hex::vec2<T> &a, const Hex::vec2<T> &b) {
    return {std::min(a.x, b.x), std::min(a.y, b.y)};
}

template <typename T>
Hex::vec2<T> std::max(const Hex::vec2<T> &a, const Hex::vec2<T> &b) {
    return {std::max(a.x, b.x), std::max(a.y, b.y)};
}