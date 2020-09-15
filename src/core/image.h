#pragma once
#include <stdlib.h>
#include <iostream>     // std::cout
#include <vector>

#include "stb_image.h"

/// <summary>
/// Creates a PPM Image
/// </summary>
/// <param name="x">width in pixel</param>
/// <param name="y">height in pixel</param>
/// <param name="data">image Data</param>
/// <returns></returns>
int createPPMImage(int x, int y, std::vector<vec3> color) {
	std::cout << "P3\n" << x << " " << y << "\n255\n";

	for (int j = 0; j < y; j++) {
		for (int i = 0; i < x; i++) {
			vec3 col = color.at(i + x*j);
			int ir = int(255.99f * col.r());
			int ig = int(255.99f * col.g());
			int ib = int(255.99f * col.b());
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
	return 0;
}

int createJPEG(int x, int y, std::vector<vec3> color) {
	return 0;
}
