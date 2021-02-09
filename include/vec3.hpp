#pragma once

#include <math.h>
#include <stdlib.h>
#include <iostream>

template <typename T = float> class vec3
{
public:
    vec3() {}
    vec3(T e0, T e1, T e2) {e[0] = e0; e[1] = e1; e[2] = e2;}
    inline T x() const {return e[0];}
    inline T y() const {return e[1];}
    inline T z() const {return e[2];}
    inline T r() const {return e[0];}
    inline T g() const {return e[1];}
    inline T b() const {return e[2];}

    inline const vec3& operator+() const {return &this;}
    inline vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]);}
    inline T operator[](int i) const {return e[i];}
    inline T& operator[](int i) {return e[i];}

#if 0
    inline vec3 operator+(const vec3 &v2) const;
    inline vec3 operator-(const vec3 &v2) const;
    inline vec3 operator*(const vec3 &v2) const;
    inline vec3 operator/(const vec3 &v2) const;
#endif

    inline vec3<T>& operator+=(const vec3<T> &v2);
    inline vec3& operator-=(const vec3 &v2);
    inline vec3& operator*=(const vec3 &v2);
    inline vec3& operator/=(const vec3 &v2);

    inline vec3& operator*=(const T &t);
    inline vec3& operator/=(const T &t);

    inline T length() const {return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);}
    inline T squared_length() const {return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];}
    inline vec3<T> unit_vector() const {return *this/length();}
    inline void make_unit_vector();

    T e[3];
};

template<typename T> inline std::istream& 
operator>>(std::istream &is, vec3<T> &t) {
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

template<typename T> inline std::ostream&
operator<<(std::ostream &os, const vec3<T> &t) {
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}

template<typename T> inline void vec3<T>::make_unit_vector() {
    T k = 1.0 / length();
    e[0] *= k; e[1] *= k; e[2] *= k;
}

template<typename T> inline vec3<T>
operator+(const vec3<T> &v1, const vec3<T> &v2) {
    return vec3<T>(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

template<typename T> inline vec3<T>
operator-(const vec3<T> &v1, const vec3<T> &v2) {
    return vec3<T>(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

template<typename T> inline vec3<T>
operator*(const vec3<T> &v1, const vec3<T> &v2) {
    return vec3<T>(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

template<typename T> inline vec3<T>
operator/(const vec3<T> &v1, const vec3<T> &v2) {
    return vec3<T>(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

/// scalar operations

template<typename T> inline vec3<T>
operator*(const vec3<T> &v, const T t) {
    return vec3<T>(t * v.e[0], t * v.e[1], t * v.e[2]);
}

template<typename T> inline vec3<T>
operator*(const T t, const vec3<T> &v) {
    return v * t;
}

template<typename T> inline vec3<T>
operator/(const vec3<T> &v, const T t) {
    return vec3<T>(v.e[0]/t, v.e[1]/t, v.e[2]/t);
}

/// Cross and dot product

template<typename T> inline T
dot(const vec3<T> &v1, const vec3<T> &v2) {
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

template<typename T> inline vec3<T>
cross(const vec3<T> &v1, const vec3<T> &v2) {
    return vec3<T>( (v1.e[1] *v2.e[2]) - (v1.e[2] * v2.e[1]),
                    (v1.e[2] *v2.e[0]) - (v1.e[0] * v2.e[2]),
                    (v1.e[0] *v2.e[1]) - (v1.e[1] * v2.e[0]));
                    
}

// Accumulators
template<typename T> inline vec3<T>&
vec3<T>::operator+=(const vec3<T> &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

template<typename T> inline vec3<T>&
vec3<T>::operator-=(const vec3<T> &v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
}

template<typename T> inline vec3<T>&
vec3<T>::operator*=(const vec3<T> &v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
}

template<typename T> inline vec3<T>&
vec3<T>::operator/=(const vec3<T> &v) {
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    return *this;
}

template<typename T> inline vec3<T>&
vec3<T>::operator*=(const T &t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

template<typename T> inline vec3<T>&
vec3<T>::operator/=(const T &t) {
    e[0] /= t;
    e[1] /= t;
    e[2] /= t;
    return *this;
}

template<typename T> inline vec3<T>
unit_vector(const vec3<T> &v)
{
    return v.unit_vector();
}

template<typename T> inline vec3<T>
reflect(const vec3<T> &vector, const vec3<T> &surface_normal) {
    return vector - 2*dot(vector,surface_normal)*surface_normal;
}

/**
 * Reflection, for glass.  The angle that glass reflects varies with angle.
 */
template<typename T> inline T
schlick(T cosine, float refractive_index) {
    float r0 = (1 - refractive_index) / (1 + refractive_index);
    r0 *= r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

/**
 * Refract, according to Snell's Law.
 *
 * Returns true if refraction happened.
 */
// TODO: It's not clear how this was derived from Snell's law.
template<typename T> inline bool
refract(const vec3<T> &v1, const vec3<T> &n, T ni_over_nt, vec3<T> &out) {
    vec3<T> uv = unit_vector(v1);
    T dt = dot(uv, n); // This is some sort of magnitude
    T discriminant = 1.0 - (ni_over_nt * ni_over_nt) * (1-dt*dt);
    if (discriminant > 0) {
        // According to https://en.wikipedia.org/wiki/Snell%27s_law#Vector_form
        // I would expect  (uv - n*dt) to be just 'dt'.  Maybe this is a
        // reduction because this later needs to be multiplied by 'n' and
        // it's already been distributed?  That explains n * sqrt(...).  And it
        // would explain n * dt if there weren't any parens there.  Did things
        // just get shuffled around form the way wikipedia has it?
        out = ni_over_nt * (uv - n*dt) - n * sqrt(discriminant);
#if 0
        // This version is eqivilant and is a closer match to what appears in
        // wikipedia.  Maybe it got reordered for efficiency or to retain
        // floating point precision?
        out = ni_over_nt * uv + n * dt - n * sqrt(discriminant);
        out = ni_over_nt * uv + n * (dt - sqrt(discriminant));

        // Another deviation I'm trying to find from the forumla on Wikipedia,
        // why operate on the unit vector form of v1 instead of v1 itself?  It
        // seems like perhaps the magnitude isn't important for this equation,
        // just direction.  But if that's true it shouldn't matter if we're
        // operating on the unit vector or the original vector.  I guess
        // creating the dot product (dt) is sensitive to the magnitude of v1
        // and we know 'n' is always a unit vector (a promise made by other
        // parts of the ray tracer).
        //
        // Sun Aug 23 12:44:02 PDT 2020
        // The newer edition of the book explains this: basically, it all
        // depends on this one identity to convert from angles to vectors:
        //
        //      a . b = |a| * |b| cos(theta)
        //
        // So
        //
        //      cos(theta) = (a.b) / (|a| * |b|)
        //
        // so we can simplify this if the vectors are already unit vectors to
        //
        //      cos(theta) = a.b
#endif
        return true;
    } else {
        return false;
    }
}

/**
 * Refract, according to Snell's law.
 *
 * This is from the newer edition of the book, which does a better job of
 * explaining how the function was actually derived, but expects certain things
 * to already be done (e.g. already calculate the unit vector).
 *
 * All vector parameters must be unit vectors.
 */
template<typename T> inline vec3<T>
refract(const vec3<T> &uv, const vec3<T> &n, T etai_over_etat)
{
    T cos_theta = dot(-uv, n);
    vec3<T> r_out_perpendicular = etai_over_etat * (uv + cos_theta * n);
    vec3<T> r_out_parallel = -sqrt(fabs(1.0 - r_out_perpendicular.length_squared())) * n;
    return r_out_perpendicular + r_out_parallel;
}

template<typename T> inline vec3<T>
random_in_unit_disk()
{
    vec3<T> p;
    do {
        // TODO fix calls to drand48, use a thread local seed
        p = (T)2.0 * vec3<T>(drand48(), drand48(), 0) - vec3<T>(1, 1, 0);
    } while (dot(p,p) >= 1);
    return p;
}
