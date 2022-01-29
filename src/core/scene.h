#include "geometry.h"
#include "material.h"


#define SPHERES_AMOUNT 10
#define RAY_AMOUNT  1500000

/* PARAMETERS */
float minRadius = 1.0;
float maxRadius = 2.0;

vec3 minBB(-10, -2, -10);
vec3 maxBB(10, 2, 10);

/* from book */
GeometryList random_scene() {
	GeometryList world;

	// ground sphere
	auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));	
	world.add(make_shared<Sphere>(1000, point3(0, -1000, 0), ground_material));
	
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {

			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
			
			if ((center - point3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {  // diffuse
					auto albedo = color::random()*color::random();
					sphere_material = make_shared<lambertian>(albedo);
					world.add(make_shared<Sphere>(0.2, center, sphere_material));
				}
				else if (choose_mat < 0.95) {  // metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<Sphere>(0.2, center, sphere_material));
				}
				else {  // glass
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<Sphere>(0.2, center, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<Sphere>(1.0, point3(0, 1, 0), material1));
	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<Sphere>(1.0, point3(-4, 1, 0), material2));
	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<Sphere>(1.0, point3(4, 1, 0), material3));

	return world;
}

GeometryList random_scene2() {
	/* Geometry*/
	GeometryList world;

	// Create Spheres
	for (int i = 0; i < SPHERES_AMOUNT; i++)
	{
		shared_ptr<material> sphere_material;

		if (i < SPHERES_AMOUNT /2)
		{
			sphere_material = make_shared<lambertian>(color(.8, .3, .3));
		}
		else {
			sphere_material = make_shared<metal>(color(.8f, .8f, .8f), 1);
		}

		double r = random_double() * (maxRadius - minRadius) + minRadius;
		vec3 c = vec3(random_double(), random_double(), random_double()) * (maxBB - minBB) + minBB;

		world.add(make_shared<Sphere>(r, c, sphere_material));
	}
	return world;
}


