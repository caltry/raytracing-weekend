#include <iostream>
#include "ray.h"

float hit_sphere(const vec3<float> &center, float radius, const ray<float> &r) {
    vec3<float> oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - (radius * radius);
    float discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
        return -1.0f;
    } else {
        return (-b - sqrt(discriminant)) / (2.0*a);
    }
}

vec3<float> color(const ray<float> &r) {
    float t = hit_sphere(vec3<float>(0,0,-1), 0.5, r);
    if (t > 0.0){
        vec3<float> N =
            unit_vector(r.point_at_parameter(t) - vec3<float>(0,0,-1));
            return 0.5f * vec3<float>(N.x()+1, N.y()+1, N.z() + 1);
    }
    vec3<float> unit_direction(unit_vector(r.direction()));
    t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0f-t) * vec3<float>(1.0,1.0,1.0) + t * vec3<float>(0.5, 0.7, 1.0);
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
