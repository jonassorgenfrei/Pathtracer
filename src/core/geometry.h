#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "ray.h"

class material; // forward declaration

struct hitRecord {
	point3 p;
	vec3 normal;

	material* mat_ptr = NULL;

	float t = 0;
	
	bool front_face;

	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
	
};

class Geometry {
public:
	
	/// <summary>
	/// Checks if a specific Ray hits the Geometry.
	/// </summary>
	/// <param name="r">The ray.</param>
	/// <param name="t_min">The t minimum.</param>
	/// <param name="t_max">The t maximum.</param>
	/// <param name="rec">The record of the intersection.</param>
	/// <returns>
	/// True if the Geometry is hit by the ray.
	/// </returns>
	virtual bool hit(const ray& r,
					float t_min,
					float t_max,
					hitRecord& rec) { return 0; };

	friend std::ostream& operator<< (std::ostream& out,
									const Geometry& mc) {
		mc.print(out);
		return out;
	}

protected:
	virtual void print(std::ostream& where) const {};
	material* mat = NULL;

private:

};

class GeometryList :
	public Geometry {
public:
	GeometryList() {};
	
	GeometryList(Geometry** l, int n) { list = l; list_size = n; }

	virtual bool hit(const ray& r, float t_min, float t_max, hitRecord& rec) {
		hitRecord temp_rec;
		bool hit_anything = false;
		float closes_so_far = t_max;	// TODO CHECK float

		for (int i = 0; i < list_size; i++)
		{
			if (list[i]->hit(r, t_min, closes_so_far, temp_rec)) {
				hit_anything = true;
				closes_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}
		return hit_anything;
	};

	Geometry** getList() {
		return list;
	}
	
	int getListSize() {
		return list_size;
	}

	bool testVisibility(ray r, float l, float t_min) {
		hitRecord rec;
		for (int i = 0; i < list_size; i++)
		{
			if (list[i]->hit(r, t_min, l, rec)) {
				return false;
			}
		}
		return true;
	}
	void print(std::ostream& os) const {
		os << "{"<< std::endl;

		for (int i = 0; i < list_size; i++)
		{
			os << "\t" << *list[i] << std::endl;
		}	
		os << "}";
	};
private:
	Geometry** list = NULL;
	int list_size = 0;
};


/// <summary>
/// Sphere class
/// </summary>
/// <seealso cref="Geometry" />
class Sphere
: public Geometry 
{
public:
	Sphere() {}

	/// <summary>
	/// Initializes a new instance of the <see cref="Sphere"/> class.
	/// </summary>
	/// <param name="r">The radius</param>
	/// <param name="c">The center</param>
	/// <param name="material">The material.</param>
	Sphere(float r, vec3 c, material* material) : radius(r), center(c) { this->mat = material; };

	bool hit(const ray& r,
		float t_min,
		float t_max,
		hitRecord& rec) {

		vec3 oc = r.origin() - this->center;

		auto a = dot(r.direction(), r.direction());
		auto b = dot(oc, r.direction());
		auto c = dot(oc, oc) - radius * radius;

		float discriminant = b * b - a * c;

		if (discriminant >= 0) {

			float temp = (-b - sqrt(discriminant)) / a;
			
			if (temp < t_max && temp > t_min) {
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - center) / radius;
				rec.mat_ptr = mat;
				return true;
			}
			
			temp = (-b + sqrt(discriminant)) / a;
			
			if (temp < t_max && temp > t_min) {
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - center) / radius;
				rec.mat_ptr = mat;
				return true;
			}
		}
		return false;
	} 

	float getRadius() {
		return radius;
	}

	vec3 getCenter() {
		return center;
	}

	void print(std::ostream& os) const {
		os << "Sphere {\tcenter:" << center << "\tradius:"<< radius << "\t}";
	}
private:
	float radius = 0;
	vec3 center;
};

#endif // !GEOMETRY_H