#include "vec.h"

vec3 simpleColorGradient(int x, int y, int i, int j) {
	float r = float(i) / float(x);
	float g = float(j) / float(y);
	float b = 0.2;

	return vec3(r, g, b);
}

/**
 * Unit sphere color gradient
 */
color colorGradient(const ray& r) {
	// gradient background
	color unit_direction = unit_vector(r.direction());
	auto t = 0.5f * (unit_direction.y() + 1.0f);

	auto white = color(1.0f, 1.0f, 1.0f);
	auto blue = vec3(0.5f, 0.7f, 1.0f);

	return lerp(white, blue, t);
}


/// <summary>
/// Creates the simple color gradient.
/// </summary>
/// <param name="image_height">Height of the image.</param>
/// <param name="image_width">Width of the image.</param>
/// <returns></returns>
std::vector<vec3> createSimpleColorGradient(int image_height,
											int image_width) {
	std::vector<vec3> colors;

	for (int j = image_height - 1; j >= 0; j--) {
		for (int i = 0; i < image_width; i++) {
			colors.push_back(simpleColorGradient(image_width, image_height, i, j));
		}
	}

	return colors;
}

/// <summary>
/// Creates the a color gradient.
/// </summary>
/// <returns>Vector-array containing vec3 color vectors for size w, h</returns>
std::vector<vec3> createColorGradient(int image_width, int image_height, float aspect_ratio) {

	std::vector<vec3> colors;

	// camera 
	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;
	auto focal_length = 1.0;

	// camera parameters
	auto origin = point3(0, 0, 0);
	auto horizontal = vec3(viewport_width, 0, 0);
	auto vertical = vec3(0, viewport_height, 0);
	auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

	for (int j = image_height - 1; j >= 0; j--) {
		// progress indicator
		std::cout << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; i++) {
			float u = float(i) / float(image_width);
			float v = float(j) / float(image_height);

			const ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			colors.push_back(colorGradient(r));
		}
	}

	return colors;
}