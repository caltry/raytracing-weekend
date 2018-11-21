#include <iostream>
#include "ray.h"
#include "sphere.h"


vec3<float> color(const ray<float> &r) {
    sphere s(vec3<float>(0,0,-1), 0.5);
    hit_record rec;
    if(s.hit(r, 0, 1, rec)) {
        return 0.5f * vec3<float>(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
    } else {
        // We did not hit the sphere, draw the background
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

    for(int j = ny-1; j >= 0; j--) {
        for(int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            ray<float> r(origin, lower_left_corner
                                + u * horizontal
                                + v * vertical);
            vec3<float> col(color(r));
            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());

            std::cout << ir << " " << ig << " " << ib << std::endl;
        }
    }
    return 0;
}
