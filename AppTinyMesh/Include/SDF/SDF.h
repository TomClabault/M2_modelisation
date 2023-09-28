#ifndef SDF_H
#define SDF_H

#include "mat.h"
#include "vec.h"

#include <functional>

enum SDF_Type
{
    SDF_UNION,
    SDF_INTERSECTION,
    SDF_DIFFERENCE,
    SDF_SMOOTH_UNION,
    SDF_SMOOTH_DIFFERENCE,
    SDF_SPHERE,
    SDF_TORUS,
};

class SDF
{
public:
    static SDF* create_sphere_sdf(const Vector& center, float radius);
    static SDF* create_sphere_sdf(const Vector& center, float radius, const Transform& transform);
    static SDF* create_torus_sdf(float inner_radius, float outer_radius);
    static SDF* create_torus_sdf(float inner_radius, float outer_radius, const Transform& transform);

    static SDF* create_difference(SDF* left_child, SDF* right_child);
    static SDF* create_smooth_union(SDF* left_child, SDF* right_child, float smooth_radius);
    static SDF* create_smooth_difference(SDF* left_child, SDF* right_child, float smooth_radius);

    float Value(const Vector& point);

private:
    float smooth_min_polynomial(float value_left, float value_right, float smooth_radius);

private:
    SDF_Type m_type;

    Vector m_sphere_center;
    float m_sphere_radius;

    float m_torus_inner_radius;
    float m_torus_outer_radius;

    Transform m_inverse_transform;
    bool m_transform_set;

    //h coefficient for the smooth union for example
    float m_smooth_sdf_coefficient;

    SDF* m_left, *m_right;
};

#endif // SDF_H
