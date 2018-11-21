#pragma once

#include "hittable.h"

class sphere: public hittable {
public:
    sphere() {};
    sphere(vec3<float> center, float radius) :
        mCenter(center),
        mRadius(radius)
        {};
    virtual bool hit(const ray<float> &r, float tmin, float tmax,
                     hit_record &rec) const;
    vec3<float> mCenter;
    float mRadius;
};

bool
sphere::hit(const ray<float> &r, float tmin, float tmax, hit_record &rec) const
{
    vec3<float> oc = r.origin() - mCenter;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - (mRadius * mRadius);
    float discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
        return false;
    } else {
        float temp = (-b - sqrt(discriminant)) / (2.0*a);
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - mCenter) / mRadius;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / (2.0*a);
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - mCenter) / mRadius;
            return true;
        }
    }
    return false;
}
