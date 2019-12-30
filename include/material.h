#pragma once

#include "vec3.hpp"
#include "hittable.h"

vec3<float> random_in_unit_sphere() {
    vec3<float> p;
    do {
        // Produce a random point in [-1,1) for all dimensions
        p = 2.0f * vec3<float>(drand48(), drand48(), drand48())
            - vec3<float>(1, 1, 1);
        // And only allow shorter ones out (ones that fit in a unit-circle)
    } while (p.squared_length() >= 1.0);
    // hmm: Why do I do this in a while-loop instead of converting the vector
    // to a unit-vector?  Perhaps this would build a bias towards the edges of
    // the circle?  I bet I could build an image to simulate this!
    return p;
}

class material {
public:
    virtual bool scatter(const ray<float> &r_in, struct hit_record &rec, 
                         vec3<float> &attenuation, ray<float> &r_out) const = 0;
};

/* Like a lambertian material, but the color changes based on where the object
 * was struck */
class debugTexture : public material {
    virtual bool scatter(const ray<float> &r_in, struct hit_record &rec, 
                         vec3<float> &attenuation, ray<float> &r_out) const
    {
        bool red    = rec.normal.x() > 0 && rec.normal.y() > 0;
        bool green  = rec.normal.x() > 0 && rec.normal.y() < 0;
        bool blue   = rec.normal.x() < 0 && rec.normal.y() > 0;
        bool black  = rec.normal.x() < 0 && rec.normal.y() < 0;
        bool white  = !(red || green || blue || black);


        float magnitude = rec.normal.z();
        if (magnitude<0) magnitude = -magnitude;
        // Do this to get some white when z axis starts showing the back.
        if (magnitude < 0.01) {
            attenuation = vec3<float>(1,1,1);
            return true;
        }

        if (red)        attenuation = magnitude * vec3<float>(1,0,0);
        else if (green) attenuation = magnitude * vec3<float>(0,1,0);
        else if (blue)  attenuation = magnitude * vec3<float>(0,0,1);
        else if (black) attenuation = magnitude * vec3<float>(0,0,0);
        else            attenuation = magnitude * vec3<float>(1,1,1);

        vec3<float> target = rec.p + rec.normal + random_in_unit_sphere();
        r_out = ray<float>(rec.p, target - rec.p);

        return true;
    }
};

class lambertian : public material {
public:
    lambertian(const vec3<float> &albedo) : m_albedo(albedo) {}

    virtual bool scatter(const ray<float> &r_in, struct hit_record &rec, 
                         vec3<float> &attenuation, ray<float> &r_out) const
    {
        // Is this 'target' the destination of a recast ray?
        // No, 'target' is more like a vector, that has no source point and no
        // known destination point yet.
        // Also, couldn't I just get rid of rec.p?  We cancel it out anyways
        // when building the ray...
        vec3<float> target = rec.p + rec.normal + random_in_unit_sphere();
        r_out = ray<float>(rec.p, target - rec.p);
        attenuation = m_albedo;
        return true;
    }

private:
    vec3<float> m_albedo;
};

class metal : public material {
public:
    metal(vec3<float> albedo, float fuzzyness = 0) :
        m_albedo(albedo),
        m_fuzzyness(fuzzyness)
        {}
    virtual bool scatter(const ray<float> &r_in, struct hit_record &rec, 
                         vec3<float> &attenuation, ray<float> &r_out) const;

private:
    vec3<float> m_albedo;
    float m_fuzzyness;
};

bool metal::scatter(const ray<float> &r_in, struct hit_record &rec, 
                    vec3<float> &attenuation, ray<float> &r_out) const
{
    vec3<float> reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    r_out = ray<float>(rec.p, reflected + (m_fuzzyness * random_in_unit_sphere()));
    attenuation = m_albedo;
    return (dot(r_out.direction(), rec.normal) > 0);
}
