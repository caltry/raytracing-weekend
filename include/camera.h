#pragma once

#include "ray.h"

class camera {
public:
    camera()
    : m_lower_left_corner(-2.0, -1.0, -1.0),
      m_horizontal(4.0, 0.0, 0.0),
      m_vertical(0.0, 2.0, 0.0),
      m_origin(0.0, 0.0, 0.0)
        {}

    ray<float> get_ray(float u, float v)
    {
        ray<float> r(m_origin, m_lower_left_corner
                               + u * m_horizontal
                               + v * m_vertical);
        return r;
    }
    
    vec3<float> m_lower_left_corner;
    vec3<float> m_horizontal;
    vec3<float> m_vertical;
    vec3<float> m_origin;
};
