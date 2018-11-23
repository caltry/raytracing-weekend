#include <iostream>
#include "ray.h"
#include "sphere.h"
#include "hittable_list.h"

vec3<float> color(const ray<float> &r, hittable *world) {
    hit_record rec;
    if (world->hit(r, 0, MAXFLOAT, rec)) {
#if 1
        return 0.5f * vec3<float>(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
#else
        bool red    = rec.normal.x() > 0 && rec.normal.y() > 0;
        bool green  = rec.normal.x() > 0 && rec.normal.y() < 0;
        bool blue   = rec.normal.x() < 0 && rec.normal.y() > 0;
        bool black  = rec.normal.x() < 0 && rec.normal.y() < 0;
        bool white  = !(red || green || blue || black);

        // Do this to get some white when z axis starts showing the back.
        float magnitude = rec.normal.z();
        if (-0.01 < magnitude && magnitude < 0.01)
            return vec3<float>(1,1,1);
        if (magnitude<0) magnitude = -magnitude;

        if (red)    return magnitude * vec3<float>(1,0,0);
        if (green)  return magnitude * vec3<float>(0,1,0);
        if (blue)   return magnitude * vec3<float>(0,0,1);
        if (black)  return magnitude * vec3<float>(0,0,0);
        else        return magnitude * vec3<float>(1,1,1);
#endif
    } else {
        // Background
        vec3<float> unit_direction(unit_vector(r.direction()));
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0f-t) * vec3<float>(1.0,1.0,1.0) + t * vec3<float>(0.5, 0.7, 1.0);
    }
}

int main() {
    int nx = 200;
    int ny = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    vec3<float> lower_left_corner(-2.0, -1.0, -1.0);
    vec3<float> horizontal(4.0, 0.0, 0.0);
    vec3<float> vertical(0, 2, 0);
    vec3<float> origin(0, 0, 0);

    sphere s(vec3<float>(0,0,-1), 0.5);
    sphere s2(vec3<float>(0.5,0, -0.5), 0.2);
    sphere floor(vec3<float>(0, -100.5, -1), 100);
    hittable *objects[] = {&s, &s2, &floor};
    hittable_list list(objects, 3);

    for(int j = ny-1; j >= 0; j--) {
        for(int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            ray<float> r(origin, lower_left_corner
                                + u * horizontal
                                + v * vertical);
            vec3<float> col(color(r, &list));
            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());

            std::cout << ir << " " << ig << " " << ib << std::endl;
        }
    }
    return 0;
}
