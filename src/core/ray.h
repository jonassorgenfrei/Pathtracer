#ifndef RAYH
#define RAYH
#include "vec.h"

class ray
{
public:
	ray() {};
	ray(const vec3& orig, const vec3& dir) {
		o = orig;
		d = dir;
	};
	vec3 origin() const { return o; }
	vec3 direction() const { return d; }
	vec3 point_at_parameter(float t) const { return o + t * d;  }

private:
	vec3 o;
	vec3 d;
};


#endif // !RAYH
