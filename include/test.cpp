#include <iostream>
#include "vec3.hpp"
#include "ray.h"

int main()
{
    const vec3<float> v = vec3<float>(1.0,2.0,3.0) + vec3<float>(1.0,2.0,3.0);
    std::cout << v << std::endl;
    vec3<float> z = v*3.0f;
    vec3<float> z1 = 3.0f*v;
    std::cout << z << std::endl;
    std::cout << z1.unit_vector() << std::endl;
    std::cout << z << std::endl;

    vec3<double> d = vec3<double>(1.0,2.0,3.0);
    std::cout << d.unit_vector() << std::endl;


    ray<float>(z, z1);
    return 0;
}
