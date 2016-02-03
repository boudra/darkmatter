#ifndef MATH_HPP
#define MATH_HPP

#include <iostream>
#include <cstdint>
#include <cmath>

#include "Core/Logger.hpp"

namespace dm {

template <class T>
const T& max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

template <typename T>
T clamp(const T& n, const T& lower, const T& upper) {
    return std::max(lower, std::min(n, upper));
}
}

#endif
