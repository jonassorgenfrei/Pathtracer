#include <stdlib.h>
#include <iostream>     // std::cout
#include <limits>       // std::numeric_limits
#include <vector>

#include "ray.h"
#include "camera.h"
#include "geometry.h"
#include "material.h"

/**
 * Output
 *		0 -   RENDER SCENE
 *		1 -	  EXPORT POINT VISIBILITY
 *		2 -   EXPORT SPHERE DATA
 */
#define OUTPUT 0

#define SPHERES_AMOUNT 10
#define RAY_AMOUNT  1500000

/* PARAMETERS */
float minRadius = 1.0;
float maxRadius = 2.0;

vec3 minBB(-10, -2, -10);
vec3 maxBB(10, 2, 10);

std::string colTerm = ",";

// Resolution
int nx = 1280;	//width Res
int ny = 800;	// height Res
int ns = 10; // samples 

// Seed for Random Samples
int seed = 0;	// random Seed

/* GLOBALS */
Geometry * world_list;

vec3 color(const ray& r, int depth) {
	hitRecord rec;
	if (world_list->hit(r, 0.001, std::numeric_limits<float>::max(), rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, depth+1);
		}
		else {
			return vec3(0, 0, 0);
		}
	} else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1);

		return lerp(vec3(1, 1, 1),  // white
					vec3(0.5f, 0.7f, 1), // blue
					t);
	}
}

Geometry* random_scene() {
	int n = 500;
	Geometry** list = new Geometry * [n+1];
	list[0] = new Sphere(1000, 
						vec3(0, -1000, 0), 
						new lambertian(vec3(0.5f, 0.5f, 0.5f)));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = rand01();
			vec3 center(a + 0.9f * rand01(), 0.2f, b + 0.9f * rand01());
			if ((center-vec3(4,0.2f, 0)).length() > 0.9f) {
				if (choose_mat < 0.8) {				// diffuse
					list[i++] = new Sphere(0.2f,
											center,
											new lambertian(vec3(rand01() * rand01(),
																rand01() * rand01(), 
																rand01() * rand01()))
											);
				} else if (choose_mat < 0.95f) {	// metal
					list[i++] = new Sphere(0.2f, center,
						new metal(vec3(0.5f * (1 + rand01()),
										0.5f * (1 + rand01()),
										0.5f * (1 + rand01())), 0.5f * rand01()));
				} else {		// glass
					list[i++] = new Sphere(0.2f, center, new dielectric(1.5));
				}
			}
		}
	}
	list[i++] = new Sphere(1,vec3(0,1,0), new dielectric(1.5));
	list[i++] = new Sphere(1,vec3(-4,1,0), new lambertian(vec3(.4f,.2f,.1f)));
	list[i++] = new Sphere(1,vec3(4,1,0), new metal(vec3(.7f,.6f,.5f), 0));

	return new GeometryList(list, i);

}

Geometry* random_scene2() {
	/* Geometry*/
	Geometry** list = new Geometry*[SPHERES_AMOUNT];

	// Create Spheres
	for (int i = 0; i < SPHERES_AMOUNT; i++)
	{
		material* mat;

		if (/*i < SPHERES_AMOUNT /2*/true)
		{
			mat = new lambertian(vec3(.8f, .3f, .3f));
			// new lambertian(vec3(.8f, .8f, 0))
		}
		else {
			// mat = new metal(vec3(.8f, .8f, .8f), 1);
			// new metal(vec3(.8f, .6f, .2f), .3f)
		}

		float r = rand01() * (maxRadius - minRadius) + minRadius;
		vec3 c = vec3(rand01(), rand01(), rand01()) * (maxBB - minBB) + minBB;

		list[i] = new Sphere(r, c, mat);
	}
	return new GeometryList(list, SPHERES_AMOUNT);
}

int main() {
	srand(seed);
		
	/* Assemble (acceleration) */
	world_list = random_scene();

	/* Camera */
	vec3 lookfrom = vec3(13, 2, 3);
	vec3 lookat = vec3(0, 0, 0);
	float dist_to_focus = 10;
	float aperture =0.1f;

	Camera cam(lookfrom,
				lookat,
				vec3(0,1,0),
				90, 
				float(nx)/float(ny),
				aperture,
				dist_to_focus);
#if(OUTPUT == 0)
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
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

			int ir = int(255.99f * col.r());
			int ig = int(255.99f * col.g());
			int ib = int(255.99f * col.b());
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
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
		
		/* RANDOM */
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
#endif 
}