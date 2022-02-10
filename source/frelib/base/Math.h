#ifndef FRELIB_MATH_H
#define FRELIB_MATH_H

namespace frelib {

/**
 * Sample documentation
 * */
template <typename T> constexpr T min(T const &a, T const &b) {
  return a <= b ? a : b;
}

template <typename T> constexpr T max(T const &a, T const &b) {
  return a <= b ? b : a;
}

template <typename T> constexpr T pow(T const &x, unsigned y) {
  T result = 1;
  for (unsigned i = 0; i < y; i++)
    result *= x;

  return result;
}

/**
 * Clamps a value between a lower and upper bound, bounds included.
 * This is equivalent to doing:
 *  min(max_val, max(min_val, val))
 *
 * Examples:
 *
 *  clamp(3, 5, 7) => 5
 *  clamp(3, 2, 7) => 3
 *  clamp(3, 9, 7) => 7
 *
 * */
template <typename T, typename U, typename W>
constexpr T clamp(T const &min_val, U const &val, W const &max_val) {
  return min(max_val, max(min_val, val));
}

template <typename T>
constexpr T round_up(T const &val, unsigned const &bound) {
  if (val % bound == 0)
    return val;

  return val - (val % bound) + bound;
}

template <typename T>
constexpr T round_down(T const &val, unsigned const &bound) {
  if (val % bound == 0)
    return val;

  return val - (val % bound);
}

// TODO: fpow, sin, cos, tan, sqrt...

} // namespace frelib

using frelib::clamp;
using frelib::max;
using frelib::min;
using frelib::pow;
using frelib::round_down;
using frelib::round_up;

#endif
