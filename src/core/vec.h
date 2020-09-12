#ifndef VEC_H
#define VEC_H

#define _USE_MATH_DEFINES
#include <cmath>

#define rand01() ((float) rand() / (RAND_MAX))
#define degree_to_radian(x)	(x*M_PI/180)
#define radian_to_degree(x) (x/M_PI*180)


#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3 {
	public:
		vec3() { e[0] = 0, e[1] = 0, e[2] = 0; }

		vec3(float e0) {
			e[0] = e0, e[1] = e0, e[2] = e0;
		}

		vec3(float e0, float e1, float e2) {
			e[0] = e0;
			e[1] = e1;
			e[2] = e2;
		}

		inline float x() const { return e[0]; }
		inline float y() const { return e[1]; }
		inline float z() const { return e[2]; }

		inline float r() const { return e[0]; }
		inline float g() const { return e[1]; }
		inline float b() const { return e[2]; }

		inline const vec3& operator+() const {
			return *this;
		};
		inline vec3 operator-() const {
			return vec3(-e[0], -e[1], -e[2]);
		};
		inline float operator[](int i) const {
			return e[i];
		};
		inline float& operator[](int i) {
			return e[i];
		};

		inline vec3& operator+=(const vec3 &v2);
		inline vec3& operator-=(const vec3 &v2);
		inline vec3& operator*=(const vec3 &v2);
		inline vec3& operator/=(const vec3 &v2);
		inline vec3& operator*=(const float t);
		inline vec3& operator/=(const float t);

		inline float length() const {
			return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
		}
		inline float squared_length() const {
			return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
		}
		/// <summary>
		/// Makes the unit vector.
		/// </summary>
		inline void make_unit_vector();

		float e[3];
};


inline std::istream& operator>>(std::istream& is, vec3& t) {
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
};

inline std::ostream& operator<<(std::ostream &os, const vec3 &t) {
	os << t.e[0] << " " << t.e[1] << " " << t.e[2];
	return os;
}

inline void vec3::make_unit_vector() {
	float k = 1 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
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

inline vec3 operator*(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] * v2.e[0],
				v1.e[1] * v2.e[1],
				v1.e[2] * v2.e[2]);
}

inline vec3 operator/(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] / v2.e[0],
				v1.e[1] / v2.e[1],
				v1.e[2] / v2.e[2]);
}

inline vec3 operator*(float t, const vec3& v) {
	return vec3(v.e[0] * t,
				v.e[1] * t,
				v.e[2] * t);
}

inline vec3 operator/(const vec3 & v, float t) {
	return vec3(v.e[0] / t,
				v.e[1] / t,
				v.e[2] / t);
}
inline vec3 operator*(const vec3& v, float t) {
	return vec3(v.e[0] * t,
				v.e[1] * t,
				v.e[2] * t);
}

inline float dot(const vec3& v1, const vec3& v2) {
	return	v1.e[0] * v2.e[0] + 
			v1.e[1] * v2.e[1] + 
			v1.e[2] * v2.e[2];
}

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

inline vec3& vec3::operator+=(const vec3& v) {
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];

	return *this;
}

inline vec3& vec3::operator*=(const vec3& v) {
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];

	return *this;
}

inline vec3& vec3::operator/=(const vec3& v) {
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];

	return *this;
}

inline vec3& vec3::operator-=(const vec3& v) {
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];

	return *this;
}

inline vec3& vec3::operator*=(const float t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline vec3& vec3::operator/=(const float t) {
	float k = 1 / t;
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;

	return *this;
}

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