#pragma once

#include "ray.h"

class camera {
public:
    // vfov is top to bottom in degrees
    camera(vec3<> lookfrom, vec3<> lookat, vec3<> vup,
           float vfov = 90, float aspect_ratio = 2,
           float aperture = 0, float focus_distance = 1):
      m_origin(lookfrom)
    {
        float theta = vfov*M_PI/180;
        float half_height = tan(theta/2); // Works because z = -1
        float half_width = aspect_ratio * half_height;

        // Is vector representing a ray from 'lookfrom' to 'lookat'.  It's at
        // the center of the viewport.
        w = unit_vector(lookfrom - lookat);
        // The side of the camera plane, perpendicular to both the path we
        // would cast a ray to 'lookat' and the top of the world.
        u = unit_vector(cross(vup, w));
        // Top of the camera plane, projecting vup onto the camera plane.
        // Note: Why don't we convert this to be a unit vector?  Probablty
        // because w and u are unit vectors, which we know are already
        // perpendicular to each-other.  Those preconditions tell us that the
        // resulting vector will also be a unit vector, so we don't need to
        // make the extra conversion.
        v = cross(w, u);

        // XXX Why does this care about focus distance?
        m_horizontal = 2 * half_width * focus_distance * u;
        m_vertical = 2 * half_height * focus_distance * v;

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
        m_lower_left_corner = lookfrom - m_horizontal/2.0f - m_vertical/2.0f - w * focus_distance;

        m_lens_radius = aperture / 2;
    }

    ray<float> get_ray(float s, float t) const
    {
        vec3<float> rd = m_lens_radius * random_in_unit_disk<float>();
        auto offset = u * rd.x() + v * rd.y();
        ray<float> r(m_origin + offset, m_lower_left_corner
                               + s * m_horizontal
                               + t * m_vertical
                               - m_origin - offset);
        return r;
    }
    
    vec3<float> m_lower_left_corner;
    vec3<float> m_horizontal;
    vec3<float> m_vertical;
    vec3<float> m_origin;
    vec3<float> u, v, w;
    float m_lens_radius;
};
