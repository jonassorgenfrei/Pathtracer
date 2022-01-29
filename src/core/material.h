#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"

// refractive indices
// air = 1.0
// glass = 1.3-1.7
// diamond = 2.4

struct hitRecord; // forward declaration

class material {
	public:
		/// <summary>
		/// Scatters the specified r in.
		/// </summary>
		/// <param name="r_in">The r in.</param>
		/// <param name="rec">The record.</param>
		/// <param name="attenuation">The attenuation.</param>
		/// <param name="scattered">The scattered.</param>
		/// <returns></returns>
		virtual bool scatter(const ray& r_in,
							 const hitRecord& rec,
							 vec3& attenuation,
							 ray& scattered) const = 0;
};

class lambertian : 
	public material {
public:
	lambertian(const color& a) : albedo(a) {};

	virtual bool scatter(
		const ray& r_in,
		const hitRecord& rec,
		color& attenuation,
		ray& scattered
	) const override {
		// note it's possible to scatter with some probability p and have attenuation be albedo/p

		// find point in unit sphere
		auto scatter_direction = rec.normal + random_in_unit_sphere();
		
		// alternative using hemisphere
		//auto scatter_direction = random_in_hemisphere(rec.normal);

		// catch degenerate scatter direction
		// if random unit vector is exactly opposite the normal vector
		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;
		
		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	};
private:
	color albedo;
};

/// <summary>
/// Material to represent reflecting material
/// </summary>
/// <seealso cref="material" />
class metal :
	public material {
	public:
		metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

		virtual bool scatter(	
			const ray& r_in,
			const hitRecord& rec,
			color& attenuation,
			ray& scattered
		) const override {
			vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
			
			// fuzz parameter to offset reflection point
			scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
			
			attenuation = albedo;
			return (dot(scattered.direction(), rec.normal) > 0);
		};
	private:
		color albedo;
		// fuzziness/perturbation.
		// radius of a sphere for choosing the randomized reflection
		double fuzz;
};

/// <summary>
/// Material to represent a reflecting/refracting material 
/// </summary>
/// <seealso cref="material" />
class dielectric :
	public material {
public:
	dielectric(double index_of_refraction) : ir(index_of_refraction) {}

	virtual bool scatter(
		const ray& r_in,
		const hitRecord& rec,
		color& attenuation, 
		ray& scattered
	) const override {
		// override attenuation
		attenuation = color(1.0, 1.0, 1.0);

		double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

		vec3 unit_direction = unit_vector(r_in.direction());

		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		// if the value is bigger then 1, the material cannot refract
		// usually inside solid objects e.g. water surface mirror
		bool cannot_refract = refraction_ratio * sin_theta > 1.0;

		vec3 direction;

		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
			direction = reflect(unit_direction, rec.normal);
		else
			direction = refract(unit_direction, rec.normal, refraction_ratio);

		scattered = ray(rec.p, direction);
		return true;
	};
public:
	double ir; // index of refraction
private:
	/// <summary>
	/// Schlick approximation reflectance
	/// </summary>
	/// <param name="cosine">The cosine.</param>
	/// <param name="ref_idx">Index of the reference.</param>
	/// <returns></returns>
	static double reflectance(double cosine, double ref_idx) {
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}

};

#endif // !MATERIAL_H
