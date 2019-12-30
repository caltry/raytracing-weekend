#include <iostream>
#include "ray.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "material.h"

vec3<float> color(const ray<float> &r, hittable *world, int depth=0) {
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        ray<float> scattered;
        vec3<float> attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)){
            return attenuation * color(scattered, world, depth+1);
        } else {
            return vec3<float>(0,0,0);
        }
    } else {
        // Background
        vec3<float> unit_direction(unit_vector(r.direction()));
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0f-t) * vec3<float>(1.0,1.0,1.0) + t * vec3<float>(0.5, 0.7, 1.0);
    }
}

#ifndef SCALE
#define SCALE 8
#endif

int main() {
    int nx = SCALE * 200;
    int ny = SCALE * 100;
    int ns = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    vec3<float> lower_left_corner(-2.0, -1.0, -1.0);
    vec3<float> horizontal(4.0, 0.0, 0.0);
    vec3<float> vertical(0, 2, 0);
    vec3<float> origin(0, 0, 0);

    sphere s1(vec3<float>(-1, 0, -1), 0.5,
              new metal(vec3<float>(0.8, 0.8, 0.8), 0.1));
    sphere s2(vec3<float>(0,0,-1), 0.5,
              new lambertian(vec3<float>(0.8, 0.3, 0.3)));
    sphere s3(vec3<float>(1, 0, -1), 0.5,
              new metal(vec3<float>(0.8, 0.6, 0.2), 0.8));
    sphere floor(vec3<float>(0, -100.5, -1), 100,
              new lambertian(vec3<float>(0.5, 0.5, 0.5)));
    hittable *objects[] = {&s1, &s2, &s3, &floor};
    hittable_list list(objects, 4);

    camera cam;

    for(int j = ny-1; j >= 0; j--) {
        for(int i = 0; i < nx; i++) {
            // Capture multiple samples within a pixel
            vec3<float> col(0,0,0);

// Make antialiasing optional for faster debug renders
#define ANTIALIAS 1
#if ANTIALIAS
            for (int s=0; s < ns; s++) {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray<float> r = cam.get_ray(u, v);
                col += color(r, &list);
            }
            col /= float(ns);
#else
            ray<float> r = cam.get_ray(i/float(nx), j/float(ny));
            col = color(r, &list);
#endif

            // Gamma correction
            col = vec3<float>(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());

            std::cout << ir << " " << ig << " " << ib << std::endl;
        }
    }
    return 0;
}
