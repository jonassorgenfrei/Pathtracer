/**
 * Library with vector functions and class
 */
#ifndef VEC_H
#define VEC_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include "common.h"

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
			double k = 1.0 / t;
			return *this *= k;
		}

		double length() const {
			return sqrt(squared_length());
		}

		double squared_length() const {
			return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
		}

		void replaceNaN() {
			if (e[0] != e[0]) e[0] = 0.0;
			if (e[1] != e[1]) e[1] = 0.0;
			if (e[2] != e[2]) e[2] = 0.0;
		}

		/// <summary>
		/// Returns true if the vector is close to zero in all dimensions
		/// </summary>
		/// <returns></returns>
		bool near_zero() const {
			const auto s = 1e-8;
			return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
		}

		inline static vec3 random() {
			return vec3(random_double(), random_double(), random_double());
		}

		inline static vec3 random(double min, double max) {
			return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
		}
	public:
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
inline std::istream& operator>>(std::istream& is, vec3& v) {
	return is >> v.e[0] >> v.e[1] >> v.e[2];
};

/**
 * Export string representation to stream
 */
inline std::ostream& operator<<(std::ostream &os, const vec3 &v) {
	return os << v.e[0] << " " << v.e[1] << " " << v.e[2];
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
inline double dot(const vec3& v1, const vec3& v2) {
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
inline vec3 lerp(vec3 v1, vec3 v2, double t) {
	return (1 - t) * v1 + t * v2;
}

/// <summary>
/// Randoms the in unit disk.
/// </summary>
/// <returns></returns>
inline vec3 random_in_unit_disk() {
	while (true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.squared_length() >= 1) continue;
		return p;
	}
}

/// <summary>
/// Returns a rando vector in the unit sphere. 
/// 
/// It calculates a point in the unit cube where x,y,z all range from -1 to +.
/// Reject this point and try again if the point is outside of the sphere.
/// </summary>
/// <returns></returns>
inline vec3 random_in_unit_sphere() {
	while (true) {
		auto p = vec3::random(-1, 1);
		if (p.squared_length() >= 1) continue;
		return p;
	}
}

/// <summary>
/// Randoms the unit vector.
/// </summary>
/// <returns></returns>
inline vec3 random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}

/// <summary>
/// Randoms the in hemisphere.
/// </summary>
/// <param name="normal">The normal.</param>
/// <returns></returns>
inline vec3 random_in_hemisphere(const vec3& normal) {
	vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0) // in the same hemisphere as the normal
		return in_unit_sphere;
	else
		return -in_unit_sphere;
}

/// <summary>
/// Reflects the specified vector using the normal.
/// </summary>
/// <param name="v">The incident vector</param>
/// <param name="n">The normal vector used to reflect.</param>
/// <returns></returns>
inline vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}

/// <summary>
/// Refracts the specified vector using the normal.
/// 
/// Using snells law.
/// 
/// Note: the incident vector and the normal vector need to be unit vectors
/// </summary>
/// <param name="v">The incident vector/ray. Unit vector</param>
/// <param name="n">The normal vector used to refract. Unit vector</param>
/// <param name="etai_over_etat">The refractive index ratio.</param>
/// <returns></returns>
inline vec3 refract(const vec3& v, const vec3& n, double etai_over_etat) {
	auto cos_theta = fmin(dot(-v, n), 1.0);
	// ray perpendicular to n'
	vec3 r_out_perp = etai_over_etat * (v + cos_theta * n);
	// rat parallel to n'
	vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.squared_length())) * n;
	return r_out_perp + r_out_parallel;
}

#endif // !VEC_H