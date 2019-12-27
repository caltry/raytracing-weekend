#pragma once

#include "ray.h"

class material;

struct hit_record {
    float t;
    // This is a point on an object relative to the entire scene.
    vec3<float> p;
    // This is a point normalized to the center of the object, not a normal
    // vector from the origin.
    vec3<float> normal;
    material *mat_ptr;
};

class hittable {
public:
    virtual bool hit(const ray<float> &r, float t_min, float t_max,
                     hit_record &rec) const = 0;
};
