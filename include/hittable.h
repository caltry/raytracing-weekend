#pragma once

#include "ray.h"

struct hit_record {
    float t;
    vec3<float> p;
    vec3<float> normal;
};

class hittable {
public:
    virtual bool hit(const ray<float> &r, float t_min, float t_max,
                     hit_record &rec) const = 0;
};
