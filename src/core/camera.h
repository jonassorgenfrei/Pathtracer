#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

/// <summary>
/// Implements an (axis aligned) camera
/// </summary>
class Camera {
	public:

		// default constructor
		Camera() : Camera(point3(0, 0, -1), point3(0, 0, 0), vec3(0, 1, 0), 40, 1, 0, 10) {};

		/// <summary>
		/// Initializes a new instance of the <see cref="Camera"/> class.
		/// </summary>
		/// <param name="lookfrom">The lookfrom.</param>
		/// <param name="lookat">The lookat.</param>
		/// <param name="vup">The vup.</param>
		/// <param name="vfov">The vertical field-of-view in degrees.  vfov is top to bottom in degrees</param>
		/// <param name="aspect">The aspect ratio.</param>
		Camera(
			point3 lookfrom,
			point3 lookat,
			vec3 vup, 
			double vfov, 
			double aspect,
			double aperture,
			double focus_dist,
			double _time0 = 0,
			double _time1 = 0) {


			auto theta = degrees_to_radians(vfov);
			auto h = tan(theta / 2);
			auto viewport_height = 2.0 * h;
			auto viewport_width = aspect * viewport_height;
			
			w = unit_vector(lookfrom - lookat);
			u = unit_vector(cross(vup, w));
			v = cross(w, u);

			origin = lookfrom;

			horizontal = focus_dist * viewport_width * u;
			vertical = focus_dist * viewport_height * v;

			lower_left_corner = origin 
								- horizontal/2 
								- vertical/2 
								- focus_dist * w;

			lens_radius = aperture / 2;
			time0 = _time0;
			time1 = _time1;
		};

		ray get_ray(double s, double t) const {
			vec3 rd = lens_radius * random_in_unit_disk();

			vec3 offset = u * rd.x() + v * rd.y();

			return ray(origin	+	offset,
						lower_left_corner + s* horizontal+ t * vertical - origin - offset,
						random_double(time0, time1));
		};
	private:
		point3 origin;
		point3 lower_left_corner;
		vec3 horizontal;
		vec3 vertical;

		vec3 u, v, w;

		double lens_radius;
		double time0, time1; // shutter open/close times

};

#endif // !CAMERA_H