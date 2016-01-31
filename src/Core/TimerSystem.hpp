#ifndef TIMERSYSTEM_HPP
#define TIMERSYSTEM_HPP

#include <chrono>
#include <vector>
#include <functional>

#include "Core/System.hpp"

namespace dm {

typedef std::chrono::high_resolution_clock clock_t;
typedef std::chrono::milliseconds ms_t;
typedef std::function<bool()> timercallback_t;

struct Timer {
    ms_t start;
    ms_t tick;
    timercallback_t function;
};

} /* namespace dm */

#endif /* end of include guard: TIMERSYSTEM_HPP */
