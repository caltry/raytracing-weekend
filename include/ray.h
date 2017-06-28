#pragma once
#include "vec3.hpp"

template<typename T> class ray {
public:
    ray() {};
    ray(const vec3<T> &a, const vec3<T> &b) : mA(a), mB(b) {}
    vec3<T> origin() const {return mA;}
    vec3<T> direction() const {return mB;}
    vec3<T> point_at_parameter(T t) const {return mA + (t * mB);}

    vec3<T> mA;
    vec3<T> mB;
};
