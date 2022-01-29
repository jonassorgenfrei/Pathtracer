#ifndef RAYH
#define RAYH

#include "vec.h"

/// <summary>
/// Class to describe rays for ray intersection tests
/// </summary>
class ray
{
	public:
		ray() {};

		ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction), tm(0) {}

		ray(const point3& origin, const vec3& direction, double time) : orig(origin), dir(direction), tm(time) {}

		point3 origin() const { 
			return orig;
		}
		
		vec3 direction() const { 
			return dir; 
		}

		double time() const {
			return tm;
		}
		
		point3 point_at_parameter(double t) const { 
			return orig + t * dir;
		}

	public:
		vec3 orig;	// origin
		vec3 dir; // direction
		double tm;
};


#endif // !RAYH
