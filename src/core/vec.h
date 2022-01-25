/**
 * Library with vector functions and class
 */
#ifndef VEC_H
#define VEC_H

#define _USE_MATH_DEFINES
#include <cmath>

// math functions defines
#define rand01() ((float) rand() / (RAND_MAX))
#define degree_to_radian(x)	(x*M_PI/180)
#define radian_to_degree(x) (x/M_PI*180)


#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3 {
	public:
		vec3() : e{0,0,0} { }
		vec3(double e0) : e{ e0, e0, e0 } {}
		vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

		// for position or direction
		double x() const { return e[0]; }
		double y() const { return e[1]; }
		double z() const { return e[2]; }

		// for color
		double r() const { return e[0]; }
		double g() const { return e[1]; }
		double b() const { return e[2]; }

		const vec3& operator+() const {
			return *this;
		};

		vec3 operator-() const {
			return vec3(-e[0], -e[1], -e[2]);
		};

		double operator[](int i) const {
			return e[i];
		};

		double& operator[](int i) {
			return e[i];
		};

		vec3& operator+=(const vec3 &v) {
			e[0] += v.e[0];
			e[1] += v.e[1];
			e[2] += v.e[2];

			return *this;
		}

		vec3& operator-=(const vec3 &v) {
			e[0] -= v.e[0];
			e[1] -= v.e[1];
			e[2] -= v.e[2];

			return *this;
		}

		vec3& operator*=(const vec3 &v) {
			e[0] *= v.e[0];
			e[1] *= v.e[1];
			e[2] *= v.e[2];

			return *this;
		}

		vec3& operator/=(const vec3& v) {
			e[0] /= v.e[0];
			e[1] /= v.e[1];
			e[2] /= v.e[2];

			return *this;
		}

		vec3& operator*=(const double t) {
			e[0] *= t;
			e[1] *= t;
			e[2] *= t;
			return *this;
		}

		vec3& operator/=(const double t) {
			float k = 1 / t;
			return *this *= k;
		}

		float length() const {
			return sqrt(squared_length());
		}
		float squared_length() const {
			return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
		}


		// Class Members 
		// -------------
		double e[3];
};

// Type aliases for vex3
using point3 = vec3;	// 3D point
using color = vec3;		// RGB color


// vec3 utility functions
// ----------------------

/**
 * Export string representation to stream
 */
inline std::istream& operator>>(std::istream& is, vec3& t) {
	return is >> t.e[0] >> t.e[1] >> t.e[2];
};

/**
 * Export string representation to stream
 */
inline std::ostream& operator<<(std::ostream &os, const vec3 &t) {
	return os << t.e[0] << " " << t.e[1] << " " << t.e[2];
}


inline vec3 operator+(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] + v2.e[0], 
				v1.e[1] + v2.e[1], 
				v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.e[0] - v2.e[0],
				v1.e[1] - v2.e[1],
				v1.e[2] - v2.e[2]);
}

// vector-vector multiplication
inline vec3 operator*(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] * v2.e[0],
				v1.e[1] * v2.e[1],
				v1.e[2] * v2.e[2]);
}

// vector-vector division
inline vec3 operator/(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] / v2.e[0],
				v1.e[1] / v2.e[1],
				v1.e[2] / v2.e[2]);
}

// scalar-vector multiplication
inline vec3 operator*(double t, const vec3& v) {
	return vec3(v.e[0] * t,
				v.e[1] * t,
				v.e[2] * t);
}

// scalar-vector division
inline vec3 operator/(const vec3 & v, double t) {
	return (1/t)*v;
}

// vector-scalar multiplication
inline vec3 operator*(const vec3& v, double t) {
	return t * v;
}

// calulates the dot (skalar) product of two vectors
inline float dot(const vec3& v1, const vec3& v2) {
	return	v1.e[0] * v2.e[0] + 
			v1.e[1] * v2.e[1] + 
			v1.e[2] * v2.e[2];
}

// calculates the cross product between two vectors
inline vec3 cross(const vec3& v1, const vec3& v2) {
	/*
	return vec3(	(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
					(-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
					(v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0])		);
	*/

	return vec3(	(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
					(v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2]),
					(v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0])		);
}

/// <summary>
/// Make unit vector (vector of length 1)
/// </summary>
inline vec3 unit_vector(vec3 v) {
	return v / v.length();
}

/// <summary>
/// Lerps (blends) the specified v1 and v2 using parameter t.
/// </summary>
/// <param name="v1">The start vec3.</param>
/// <param name="v2">The end v2.</param>
/// <param name="t">The pblend arameter t.</param>
/// <returns></returns>
inline const vec3 lerp(vec3 v1, vec3 v2, float t) {
	return (1 - t) * v1 + t * v2;
}

/// <summary>
/// Reflects the specified vector using the normal.
/// </summary>
/// <param name="v">The incident vector</param>
/// <param name="n">The normal vector used to reflect.</param>
/// <returns></returns>
inline const vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}


inline const vec3 unitRand() {
	return unit_vector(vec3(rand01()*2-1, rand01()*2-1, rand01()*2-1));
}


inline vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2 * vec3((float)rand01(), (float)rand01(), (float)rand01()) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1);
	return p;
}

#endif // !VEC_H