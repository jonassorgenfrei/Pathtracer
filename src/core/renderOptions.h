#include <stdlib.h>
#include <iostream>     // std::cout

/**
* Image parameters
*/
class RenderOption {
public:
	std::string outputPath = "img.ppm";

	// image 
	float aspect_ratio = 16.0 / 9.0;

	// Resolution
	int image_width = 1920;		// width
	int image_height = static_cast<int>(image_width / aspect_ratio);	// height

	int samples = 10; // samples per pixel

	// Seed for Random Samples
	int seed = 0;	// random Seed

	// empty constructor
	RenderOption() {
	}
};