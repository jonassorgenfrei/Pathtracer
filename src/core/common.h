#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory> // for shared ptr

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// utility functions

/// <summary>
/// Converts degreeses to radians.
/// </summary>
/// <param name="degrees">The degrees.</param>
/// <returns></returns>
inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

/// <summary>
/// Converts radians to degreeses.
/// </summary>
/// <param name="degrees">The degrees.</param>
/// <returns></returns>
inline double radians_to_degrees(double radians) {
	return radians / pi * 180.0;
}

/// <summary>
/// Clamps the specified value x to the range [minimum, maximum].
/// </summary>
/// <param name="x">The value x.</param>
/// <param name="min">The minimum.</param>
/// <param name="max">The maximum.</param>
/// <returns></returns>
inline double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

/// <summary>
/// Returns a random real in [0,1)
/// </summary>
/// <returns></returns>
inline double random_double() {
	return rand() / (RAND_MAX + 1.0);
}

/* alternative random number generator cpp */
/*
#include <random>
inline double random_double() {
	static std::uniform_int_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}
*/

/// <summary>
/// Returns a random real in [minimum, maximum)
/// </summary>
/// <param name="min">The minimum.</param>
/// <param name="max">The maximum.</param>
/// <returns></returns>
inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

/// <summary>
/// Returns a random integerin in [minimum, maximum]
/// </summary>
/// <param name="min">The minimum.</param>
/// <param name="max">The maximum.</param>
/// <returns></returns>
inline int random_int(int min, int max) {
	return static_cast<int>(random_double(min, max + 1));
}

// common headers
#include "ray.h"
#include "vec.h"

#endif // ! COMMON_H
