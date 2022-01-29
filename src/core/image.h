#pragma once

#include <fstream>
#include <iostream>     // std::cout
#include <stdlib.h>
#include <vector>

// Disable pedantic warnings for this external library.
#ifdef _MSC_VER
	// Microsoft Visual C++ Compiler
#pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Restore warning levels.
#ifdef _MSC_VER
	// Microsoft Visual C++ Compiler
#pragma warning (pop)
#endif

/// <summary>
/// Creates a PPM Image
/// </summary>
/// <param name="x">width in pixel</param>
/// <param name="y">height in pixel</param>
/// <param name="color">color image Data</param>
/// <returns></returns>
int createPPM(std::string filePath, int x, int y, std::vector<vec3> color) {
	
	std::ofstream outFile;
	outFile.open(filePath);
	outFile << "P3\n" << x << " " << y << "\n255\n";
	for (int j = 0; j < y; j++) {
		for (int i = 0; i < x; i++) {
			vec3 col = color.at(i + x * j);
			int ir = int(255.99f * col.r());
			int ig = int(255.99f * col.g());
			int ib = int(255.99f * col.b());
			outFile << ir << " " << ig << " " << ib << "\n";
		}
	}
	outFile.close();

	return 0;
}

/// <summary>
/// Creates a JPEG image
/// </summary>
/// <param name="filePath">The file path.</param>
/// <param name="x">width in pixel</param>
/// <param name="y">height in pixel</param>
/// <param name="color">color image Data</param>
/// <returns></returns>
int createJPEG(std::string filePath, int x, int y, std::vector<vec3> color) {
	unsigned char * data = (unsigned char* ) malloc(x * y * 3 * sizeof(unsigned char));
	int idx = 0;
	for (int j = y-1; j >= 0; --j) {
		for (int i = 0; i < x; i++) {
			vec3 col = color.at(i + x * j);

			data[i * 3 + x * j * 3]     = (unsigned char) (255.0f * col.r());
			data[i * 3 + x * j * 3 + 1] = (unsigned char) (255.0f * col.g());
			data[i * 3 + x * j * 3 + 2] = (unsigned char) (255.0f * col.b());
		}
	}
	
	stbi_write_jpg(filePath.c_str(), x, y, 3, data, x * 3);
	free(data);
	return 1;
}

/// <summary>
/// Creates the PNG
/// </summary>
/// <param name="filePath">The file path.</param>
/// <param name="x">width in pixel</param>
/// <param name="y">height in pixel</param>
/// <param name="color">The color.</param>
/// <returns>color image Data</returns>
int createPNG(std::string filePath, int x, int y, std::vector<vec3> color) {
	unsigned char* data = (unsigned char*)malloc(x * y * 3 * sizeof(unsigned char));
	int idx = 0;
	for (int j = y - 1; j >= 0; --j) {
		for (int i = 0; i < x; i++) {
			vec3 col = color.at(i + x * j);

			data[i * 3 + x * j * 3] = (unsigned char)(255.0f * col.r());
			data[i * 3 + x * j * 3 + 1] = (unsigned char)(255.0f * col.g());
			data[i * 3 + x * j * 3 + 2] = (unsigned char)(255.0f * col.b());
		}
	}

	stbi_write_png(filePath.c_str(), x, y, 3, data, x * 3);
	free(data);
	return 1;
}
