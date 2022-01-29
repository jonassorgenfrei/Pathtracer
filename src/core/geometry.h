#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "common.h"

#include <vector>

class material; // forward declaration

struct hitRecord {
	point3 p;
	vec3 normal;
	shared_ptr<material> mat_ptr;
	double t;
	bool front_face;

	/// <summary>
	/// Checks if the intersection is on the front or backface of an object.
	/// </summary>
	/// <param name="r">The r.</param>
	/// <param name="outward_normal">The outward normal.</param>
	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0; // checks if the ray incident is on the front or backface
		normal = front_face ? outward_normal : -outward_normal; // inverts the normal if the incident in on the backface
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
						 double t_min,
						 double t_max,
						 hitRecord& rec) const = 0;

		friend std::ostream& operator<< (std::ostream& out,
										 const Geometry& mc) {
			mc.print(out);
			return out;
		}

protected:
	virtual void print(std::ostream& where) const {};
	
};

/// <summary>
/// Stores a list of Geometry.
/// 
/// It uses shared_ptr, whch is a pointer to some allocated type, with refernce-counting semantics
/// Everytime the value is assigned to another shared pointer, the ref. count is incremented
/// When going out of scope, the refence count is decremented. (When going to zero, the object is deleted)
/// </summary>
/// <seealso cref="Geometry" />
class GeometryList : public Geometry {
public:
	GeometryList() {};
	
	GeometryList(shared_ptr<Geometry> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<Geometry> object) { objects.push_back(object); }


	/// <summary>
	/// Checks if the ray r hits the geometry in the geometry list
	/// </summary>
	/// <param name="r">The ray.</param>
	/// <param name="t_min">The t minimum.</param>
	/// <param name="t_max">The t maximum.</param>
	/// <param name="rec">The record.</param>
	/// <returns></returns>
	virtual bool hit(const ray& r, double t_min, double t_max, hitRecord& rec) const override;

	//Geometry** getList() {
	//	return list;
	//}
	
	
	int size() {
		return objects.size();
	}
	

	/*bool testVisibility(ray r, float l, float t_min) {
		hitRecord rec;
		for (int i = 0; i < list_size; i++)
		{
			if (list[i]->hit(r, t_min, l, rec)) {
				return false;
			}
		}
		return true;
	}*/
	void print(std::ostream& os) const {
		os << "{"<< std::endl;

		for (const auto& object : objects)
		{
			os << "\t" << object << std::endl;
		}	
		os << "}";
	};
	
private:
	std::vector<shared_ptr<Geometry>> objects;
};


bool GeometryList::hit(const ray& r, double t_min, double t_max, hitRecord& rec) const {
	hitRecord temp_rec;
	auto hit_anything = false;
	auto closes_so_far = t_max;

	for (const auto& object : objects)
	{
		if (object->hit(r, t_min, closes_so_far, temp_rec)) {
			hit_anything = true;
			closes_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
};

// ------------------------------------------------------------------------------
// Geometry Implementations
// ------------------------------------------------------------------------------

/// <summary>
/// Sphere class
/// </summary>
/// <seealso cref="Geometry" />
class Sphere : public Geometry {
public:
	Sphere() {}

	/// <summary>
	/// Initializes a new instance of the <see cref="Sphere"/> class.
	/// </summary>
	/// <param name="r">The radius</param>
	/// <param name="c">The center</param>
	/// <param name="material">The material.</param>
	Sphere(double r, point3 c, shared_ptr<material> material) : radius(r), center(c), mat_ptr(material){};

	virtual bool hit(const ray& r,
					double t_min,
					double t_max,
					hitRecord& rec) const override;
	
	double getRadius() {
		return radius;
	}

	point3 getCenter() {
		return center;
	}

	void print(std::ostream& os) const {
		os << "Sphere {\tcenter:" << center << "\tradius:"<< radius << "\t}";
	}
private:
	double radius = 0;
	point3 center;
	shared_ptr<material> mat_ptr;
};

bool Sphere::hit(const ray& r,
				 double t_min,
				 double t_max,
				 hitRecord& rec) const
{

	vec3 oc = r.origin() - center;

	auto a = r.direction().squared_length();
	auto half_b = dot(oc, r.direction());
	auto c = oc.squared_length() - radius * radius;

	auto discriminant = half_b * half_b - a * c;

	if (discriminant < 0) return false;

	auto sqrtd = sqrt(discriminant);
	
	// find the nearest root that lies in the acceptable range
	auto root = (-half_b - sqrtd) / a;

	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root)
			return false;
	}

	rec.t = root;
	rec.p = r.point_at_parameter(rec.t);
	// calculate normal at intersection time
	vec3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal);
	rec.mat_ptr = mat_ptr;
	
	return true;
}


#endif // !GEOMETRY_H