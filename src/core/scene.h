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
Geometry* random_scene() {

	int n = 500;
	Geometry** list = new Geometry * [n + 1];
	
	auto ground_material = new lambertian(vec3(0.5f, 0.5f, 0.5f));
	
	// ground sphere
	list[0] = new Sphere(1000,
						point3(0, -1000, 0),
						ground_material);
	
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = rand01();
			point3 center(a + 0.9f * rand01(), 0.2f, b + 0.9f * rand01());
			
			if ((center - vec3(4, 0.2f, 0)).length() > 0.9f) {
				if (choose_mat < 0.8) {  // diffuse
					auto albedo = rand01() * rand01();
					list[i++] = new Sphere(0.2f,
											center,
											
											new lambertian(color(albedo))
					);
				}
				else if (choose_mat < 0.95f) {  // metal
					list[i++] = new Sphere(0.2f, center,
						new metal(color(0.5f * (1 + rand01()),
							0.5f * (1 + rand01()),
							0.5f * (1 + rand01())), 0.5f * rand01()));
				}
				else {  // glass
					list[i++] = new Sphere(0.2f, center, new dielectric(1.5));
				}
			}
		}
	}
	list[i++] = new Sphere(1, point3(0, 1, 0), new dielectric(1.5));
	list[i++] = new Sphere(1, point3(-4, 1, 0), new lambertian(color(.4f, .2f, .1f)));
	list[i++] = new Sphere(1, point3(4, 1, 0), new metal(color(.7f, .6f, .5f), 0));

	return new GeometryList(list, i);

}

Geometry* random_scene2() {
	/* Geometry*/
	Geometry** list = new Geometry * [SPHERES_AMOUNT];

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


