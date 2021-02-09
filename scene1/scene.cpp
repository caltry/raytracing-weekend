#include <future>
#include <iostream>
#include <list>
#include <vector>
#include "ray.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "material.h"

vec3<float> color(const ray<float> &r, const hittable *world, int depth=0) {
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

#ifndef PARALLEL
#define PARALLEL 1
#endif

#ifndef ANTIALIAS
#define ANTIALIAS 1
#endif

thread_local unsigned short rand_seed[3] = {0x1234, 0xabcd, 0x330e};

vec3<>
render_pixel(const camera &cam, const hittable_list &objects,
             int j, int i, int ny, int nx)
{
    // Capture multiple samples within a pixel
    vec3<float> col(0,0,0);
    int ns = 100;

// Make antialiasing optional for faster debug renders
#if ANTIALIAS
    for (int s=0; s < ns; s++) {
        float u = float(i + erand48(rand_seed)) / float(nx);
        float v = float(j + erand48(rand_seed)) / float(ny);
        ray<float> &&r = cam.get_ray(u, v);
        col += color(r, &objects);
    }
    col /= float(ns);
#else
    ray<float> r = cam.get_ray(i/float(nx), j/float(ny));
    col = color(r, &objects);
#endif

    // Gamma correction
    return vec3<float>(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
}

std::list<vec3<> >
render_row(const camera &cam, const hittable_list &objects, int ny, int nx, int row)
{
    std::list<vec3<> > output;
    for(int i = 0; i < nx; i++) {
        output.push_back(render_pixel(cam, objects, row, i, ny, nx));
    }

    return output;
}

std::list<std::vector<vec3<> > >
render_rows(const camera &cam, const hittable_list &objects, int ny, int nx,
            int startRow, int endRow)
{
    int numRows = endRow-startRow;
    std::list<std::vector<vec3<> > > output;
    int rowMultiplier = 0;
    for(int j = endRow-1; j >= startRow; j--) {
        std::vector<vec3<> > row(nx);
        for(int i = 0; i < nx; i++) {
            row[i] = render_pixel(cam, objects, j, i, ny, nx);
        }
        output.push_back(row);
        rowMultiplier += nx;
    }

    return output;
}

inline void
drawPixel(const vec3<> &pixel)
{
    int ir = int(255.99 * pixel.r());
    int ig = int(255.99 * pixel.g());
    int ib = int(255.99 * pixel.b());

    std::cout << ir << " " << ig << " " << ib << std::endl;
}

std::list<vec3<> >
render_parallel(const camera &cam, const hittable_list &objects, int ny, int nx)
{
    typedef std::list<std::vector<vec3<> > > RenderedRow;
    std::list<std::future<RenderedRow> > futures;
    int threads = 4;
    int rowsPerThread = ny / threads;
    for(int j = ny-1; j >= 0; j -= rowsPerThread) {
        futures.push_back(std::async
            (render_rows, cam, objects, ny, nx, j + 1 - rowsPerThread, j+1));
        fprintf(stderr, "render_parallel: started future\n");
    }

    for(auto f = futures.begin(); f != futures.end(); f++) {
        auto &&batch = f->get();
        for(auto row = batch.begin(); row != batch.end(); row++) {
            for(auto pixel = row->begin(); pixel != row->end(); pixel++) {
                drawPixel(*pixel);
            }
        }
        fprintf(stderr, "render_parallel: processed future\n");
    }

    return std::list<vec3<> >();
}

std::list<vec3<> >
render(const camera &cam, const hittable_list &objects, int ny, int nx)
{
    std::list<vec3<> > output;
    for(int j = ny-1; j >= 0; j--) {
        for(int i = 0; i < nx; i++) {
            output.push_back(render_pixel(cam, objects, j, i, ny, nx));
        }
    }

    return output;
}

int main() {
    int nx = SCALE * 200;
    int ny = SCALE * 100;
    int ns = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    vec3<float> lower_left_corner(-2.0, -1.0, -1.0);
    vec3<float> horizontal(4.0, 0.0, 0.0);
    vec3<float> vertical(0, 2, 0);
    vec3<float> origin(0, 0, 0);

#if 0
    // This is basically the first scene, but modified as new materials were
    // developed, at least through chapter 8.
    sphere s1(vec3<float>(-1, 0, -1), 0.5,
              new metal(vec3<float>(0.8, 0.8, 0.8), 0.1));
    sphere s2(vec3<float>(0,0,-1), 0.5,
              new lambertian(vec3<float>(0.8, 0.3, 0.3)));
#if 0
    sphere s3(vec3<float>(1, 0, -1), 0.5,
              new metal(vec3<float>(0.8, 0.6, 0.2), 0.8));
#else
    sphere s3(vec3<float>(1, 0, -1), 0.5, new dielectric(1.5));
#endif
    sphere floor(vec3<float>(0, -100.5, -1), 100,
              new lambertian(vec3<float>(0.5, 0.5, 0.5)));
    hittable *objects[] = {&s1, &s2, &s3, &floor};
#elif 1
    // This is a scene that shows off refraction.
	sphere s1(vec3<>(0,0,-1), 0.5, new lambertian(vec3<>(0.1, 0.2, 0.5)));
	sphere s2(vec3<>(1,0,-1), 0.5, new metal(vec3<>(0.8, 0.6, 0.2), 0.0));
	sphere s3(vec3<>(-1,0,-1), 0.5, new dielectric(1.5));
	sphere s4(vec3<>(-1,0,-1), -0.45, new dielectric(1.5)); // inside of bubble
	sphere floor(vec3<>(0,-100.5,-1), 100, new lambertian(vec3<>(1.8, 0.8, 0.0)));
    hittable *objects[] = {&s1, &s2, &s3, &s4, &floor};
#endif

    hittable_list list(objects, sizeof(objects)/sizeof(*objects));

#if 0
    // This is the default camera that was used in earlier chapters.
    camera cam(vec3<>(0, 0, 0), // lookfrom
               vec3<>(0, 0, -1), // lookat
               vec3<>(0.0, 1.0, 0.0), // vup (twisting the camera)
               90, float(nx)/float(ny));
#elif 0
    // This is a different, more interesting camera angle introduced in
    // chapter 10.
    camera cam(vec3<>(-2, 2, 1), // lookfrom
               vec3<>(0, 0, -1), // lookat
               vec3<>(0.0, 1.0, 0.0), // vup (twisting the camera)
               90, float(nx)/float(ny));
#elif 1
    // Defocus blur scene
	vec3<> lookfrom(3,3,2);
	vec3<> lookat(0,0,-1);
	vec3<> vup(0,1,0);
	auto dist_to_focus = (lookfrom-lookat).length();
	auto aperture = 2.0;

	camera cam(lookfrom, lookat, vup, 20, 2.0, aperture, dist_to_focus);
#else
    // This angle has a really low FOV but zoomed out quite far.  It seems to
    // have interesting visual artifacts that seems like aliasing of some sort.
    // I'm curious what the cause of them is: maybe limited resolution based on
    // floating point precision?  If antialiasing is disabled, it seems like
    // the issue is mostly around recast rays, not on the initial cast.  I
    // wonder if there's some bug (or bias) in the code that's supposed to
    // recast with some randomness.
    camera cam(vec3<>(0, 0, 100), // lookfrom
               vec3<>(0, 0, -1), // lookat
               vec3<>(0.0, 1.0, 0.0), // vup (twisting the camera)
               1, float(nx)/float(ny));
#endif

#if PARALLEL
    auto frame = render_parallel(cam, list, ny, nx);
#else
    for(int j = ny-1; j >= 0; j--) {
        for(int i = 0; i < nx; i++) {
            // Capture multiple samples within a pixel
            vec3<> col = render_pixel(cam, list, j, i, ny, nx);

            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());

            std::cout << ir << " " << ig << " " << ib << std::endl;
        }
    }
#endif
    return 0;
}
