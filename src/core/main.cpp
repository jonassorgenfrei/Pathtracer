#include <stdlib.h>
#include <iostream>     // std::cout
#include <iterator>
#include <limits>       // std::numeric_limits
#include <vector>

// boost
#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include <boost/algorithm/string/predicate.hpp>

#include "ray.h"
#include "camera.h"
#include "geometry.h"
#include "material.h"
#include "renderOptions.h"
#include "scene.h"
#include "image.h"
#include "texture.h"

/**
 * Output
 *		0 -   RENDER SCENE
 *		1 -	  EXPORT POINT VISIBILITY
 *		2 -   EXPORT SPHERE DATA
 */
//#define OUTPUT 0
#define RAY_BOUNCE_LIMIT 50 // depth

//std::string colTerm = ",";

/* GLOBALS */
Geometry * world_list;
RenderOption rO;


// ray intersection
color ray_color(const ray& r, int depth) {
	// TODO: add world_list as parameter
	hitRecord rec;

	// ray bounce limit
	if (depth >= RAY_BOUNCE_LIMIT)
		return color(0, 0, 0);

	if (world_list != nullptr && 
		world_list->hit(r, 0.001, std::numeric_limits<float>::max(), rec)) {
		
		ray scattered;
		vec3 attenuation;
		
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * ray_color(scattered, depth+1);
		} else {
			return vec3(0, 0, 0);
		}
	} else {
		return colorGradient(r);
	}
}


std::vector<vec3> renderScene() {
	std::vector<vec3> colors;

	srand(rO.seed);

	/* Assemble (acceleration) */
	world_list = random_scene();

	/* Camera */
	point3 lookfrom(13, 2, 3);
	point3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture =0.1;

	// initialize camera 
	Camera cam(lookfrom,
				lookat,
				vup,
				20,
				rO.aspect_ratio,
				aperture,
				dist_to_focus);
	
	for (int j = rO.image_height - 1; j >= 0; --j) {
		// progress indicator
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < rO.image_width; ++i) {
			color pixel_color(0, 0, 0);
			for (int s = 0; s < rO.samples; ++s) {
				auto u = (i + rand01()) / (rO.image_width - 1);
				auto v = (j + rand01()) / (rO.image_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, 0);
			}
			
			pixel_color /= float(rO.samples);

			//  gamma 2 correction 
			pixel_color = vec3(sqrt(pixel_color[0]), sqrt(pixel_color[1]), sqrt(pixel_color[2]));

			colors.push_back(pixel_color);
		}
	}

	return colors;
}

/// <summary>
/// Main Entry Point
/// </summary>
/// <returns>if programm ran sucessfully</returns>
int main(int ac, char* av[]){
	
	//try {
		// define and read in programm options 
		po::options_description desc("Raytracer\nAllowed command line options");
		
		desc.add_options()
			("help", "produce help message")
			("out", po::value<std::string>(), "set the output path for the rendering")
			("width", po::value<int>(), "width of the result image")
			("height", po::value<int>(), "height of the result image")
			("samples", po::value<int>(), "samples of the result image")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(ac, av, desc), vm);
		po::notify(vm);

		if (vm.count("help")) {
			std::cout << desc << "\n";
			return 0;
		}

		if (vm.count("out")) {
			rO.outputPath = vm["out"].as<std::string>();
		} else {
			std::cout << "out was not set.\n";
		}

		if (vm.count("width")) {
			rO.image_width = std::max(0, vm["width"].as<int>());
			// todo fix settings based on image_width
		}

		if (vm.count("height")) {
			rO.image_height = std::max(0, vm["height"].as<int>());
			// todo fix settings based on image_height
		}

		if (vm.count("samples")) {
			rO.samples = std::max(0, vm["samples"].as<int>());
		}

		// MAIN PROGRAM
		//std::vector<vec3> colors = createSimpleColorGradient(rO.image_height, rO.image_width);
		std::vector<vec3> colors = renderScene();

		// run programm 
		if (boost::algorithm::ends_with(rO.outputPath, ".ppm")) {
			createPPM(rO.outputPath, rO.image_width, rO.image_height, colors);
		}
		else if (boost::algorithm::ends_with(rO.outputPath, ".jpg")) {
			createJPEG(rO.outputPath, rO.image_width, rO.image_height, colors);
		}
		else if (boost::algorithm::ends_with(rO.outputPath, ".png")) {
			createPNG(rO.outputPath, rO.image_width, rO.image_height, colors);
		}
	/*}
	catch (std::exception& e) {
		std::cerr << "error: " << e.what() << "\n";
		return 1;
	}
	catch (...) {
		std::cerr << "Exception of unknown type!\n";
	}*/

/*
#if(OUTPUT == 0)
	std::vector<vec3> imageData;
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0);
			for (int s = 0; s < ns; s++) {
				float u = float(i + rand01()) / float(nx);
				float v = float(j + rand01()) / float(ny);
				ray r = cam.get_ray(u,v);
				col += color(r,0);
			}
			
			col /= float(ns);

			//  gamma 2 correction 
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			imageData.push_back(col);
		}
	}

	createPPMImage(nx, ny, imageData);

#elif(OUTPUT==1)

	std::cout	<< "\"A.x\"" << colTerm 
				<< "\"A.y\"" << colTerm
				<< "\"A.z\"" << colTerm
				<< "\"B.x\"" << colTerm 
				<< "\"B.y\"" << colTerm
				<< "\"B.z\"" << colTerm
				<< "\"d.x\"" << colTerm 
				<< "\"d.y\"" << colTerm
				<< "\"d.z\"" << colTerm
				<< "\"l\"" << colTerm 
				<< "\"v\"" << std::endl;

	GeometryList* wL = (GeometryList*)world_list;
	Sphere** spheres = (Sphere**)wL->getList();
	int sphereCount = wL->getListSize();

	for (int i = 0; i < RAY_AMOUNT; i++) {
		vec3 A;
		vec3 B;
		float l;
		
		// RANDOM
#if FALSE
		A = vec3(rand01(), rand01(), rand01())
			* (maxBB - minBB) + minBB;
		B = vec3(rand01(), rand01(), rand01())
			* (maxBB - minBB) + minBB;

		//A = vec3(0,0,0);
		//B = vec3(4,3,0);

		l = (B - A).length();

		while (l<0.001) {
			A = vec3(rand01(), rand01(), rand01())
				* (maxBB - minBB) + minBB;
			B = vec3(rand01(), rand01(), rand01())
				* (maxBB - minBB) + minBB;

			l = (B - A).length();
		}
#elif FALSE
		int s1 = round(rand01() * (sphereCount-1));
		int s2 = round(rand01() * (sphereCount-1));
		while (s1 == s2) 
			s2 = round(rand01() * (sphereCount-1));

		float zFightOffset = 0.00001f;
		vec3 Na = unitRand() * (spheres[s1]->getRadius() + zFightOffset);
		vec3 Nb = unitRand() * (spheres[s2]->getRadius() + zFightOffset);
		vec3 Ca = spheres[s1]->getCenter();
		vec3 Cb = spheres[s2]->getCenter();

		A = Ca+Na;
		B = Cb+Nb;

		l = (B - A).length();
#else 
		int s1 = round(rand01() * (sphereCount - 1));
		int s2 = round(rand01() * (sphereCount - 1));
		while (s1 == s2) 
			s2 = round(rand01() * (sphereCount - 1));

		
		float zFightOffset = 0.00001f;

		vec3 Ca = spheres[s1]->getCenter();
		vec3 Cb = spheres[s2]->getCenter();

		vec3 AB = Cb - Ca;
		
		vec3 unitAB = unit_vector(AB);
		vec3 unitBA = -unitAB;

		vec3 sampleA = unitRand() * (spheres[s1]->getRadius() + zFightOffset);
		vec3 sampleB = unitRand() * (spheres[s2]->getRadius() + zFightOffset);

		while (dot(unitAB, unit_vector(sampleA)) <= 0) {
			sampleA = unitRand() * (spheres[s1]->getRadius() + zFightOffset);
		}

		while (dot(unitBA, unit_vector(sampleB)) <= 0) {
			sampleB = unitRand() * (spheres[s2]->getRadius() + zFightOffset);
		}

		A = Ca+sampleA;
		B = Cb+sampleB;

		l = (B - A).length();
#endif	
		ray r(A, unit_vector(B - A));

		bool h = wL->testVisibility(r, l, 0.001);

		std::cout	<< A[0] << colTerm 
					<< A[1] << colTerm
					<< A[2] << colTerm
					<< B[0] << colTerm
					<< B[1] << colTerm
					<< B[2] << colTerm
					<< r.direction()[0] << colTerm
					<< r.direction()[1] << colTerm
					<< r.direction()[2] << colTerm
					<< l << colTerm 
					<< h << std::endl;
	}
#elif(OUTPUT==2)
	std::cout << *world_list << std::endl;
#endif */

	return 0;

}