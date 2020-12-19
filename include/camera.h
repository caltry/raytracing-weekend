#pragma once

#include "ray.h"

class camera {
public:
    // vfov is top to bottom in degrees
    camera(vec3<> lookfrom, vec3<> lookat, vec3<> vup,
           float vfov = 90, float aspect_ratio = 2):
      m_origin(lookfrom)
    {
        float theta = vfov*M_PI/180;
        float half_height = tan(theta/2); // Works because z = -1
        float half_width = aspect_ratio * half_height;

        // Is vector representing a ray from 'lookfrom' to 'lookat'.  It's at
        // the center of the viewport.
        vec3<> w = unit_vector(lookfrom - lookat);
        // The side of the camera plane, perpendicular to both the path we
        // would cast a ray to 'lookat' and the top of the world.
        vec3<> u = unit_vector(cross(vup, w));
        // Top of the camera plane, projecting vup onto the camera plane.
        // Note: Why don't we convert this to be a unit vector?  Probablty
        // because w and u are unit vectors, which we know are already
        // perpendicular to each-other.  Those preconditions tell us that the
        // resulting vector will also be a unit vector, so we don't need to
        // make the extra conversion.
        vec3<> v = cross(w, u);

        m_horizontal = 2 * half_width * u;
        m_vertical = 2 * half_height * v;

        // Since we're not looking straight ahead anymore, if we pan around we
        // need to move the lower left corner of the viewport so we're pivoting
        // around the center of the camera plane, not the lower left.  It can't
        // just be fixed, so we make it reative to the 'lookfrom' point:
        // - we move it left along u since that's the left-right vector for the
        //   viewport plane.
        // - we move it down along v since that's the up-down vector for the
        //   viewport plane.
        // - We move it back along w since that affects how far the center of
        //   the scene is from the camera (closer-further).
        m_lower_left_corner = lookfrom - half_width*u - half_height*v - w;
    }

    ray<float> get_ray(float s, float t) const
    {
        ray<float> r(m_origin, m_lower_left_corner
                               + s * m_horizontal
                               + t * m_vertical
                               - m_origin);
        return r;
    }
    
    vec3<float> m_lower_left_corner;
    vec3<float> m_horizontal;
    vec3<float> m_vertical;
    vec3<float> m_origin;
};
