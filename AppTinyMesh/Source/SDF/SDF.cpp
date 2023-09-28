#include "SDF.h"

#include <cmath>

SDF* SDF::create_sphere_sdf(const Vector& center, float radius)
{
    SDF* sdf = new SDF;

    sdf->m_sphere_center = center;
    sdf->m_sphere_radius = radius;
    sdf->m_transform_set = false;
    sdf->m_type = SDF_SPHERE;

    return sdf;
}

SDF* SDF::create_sphere_sdf(const Vector& center, float radius, const Transform& transform)
{
    SDF* sdf = new SDF;

    sdf->m_sphere_center = center;
    sdf->m_sphere_radius = radius;
    sdf->m_transform_set = true;
    sdf->m_type = SDF_SPHERE;

    return sdf;
}

SDF* SDF::create_torus_sdf(float inner_radius, float outer_radius)
{
    SDF* sdf = new SDF;

    sdf->m_torus_inner_radius = inner_radius;
    sdf->m_torus_outer_radius = outer_radius;
    sdf->m_transform_set = false;
    sdf->m_type = SDF_TORUS;

    return sdf;
}

SDF* SDF::create_torus_sdf(float inner_radius, float outer_radius, const Transform& transform)
{
    SDF* sdf = new SDF;

    sdf->m_torus_inner_radius = inner_radius;
    sdf->m_torus_outer_radius = outer_radius;
    sdf->m_inverse_transform = transform.inverse(),
    sdf->m_transform_set = true;
    sdf->m_type = SDF_TORUS;

    return sdf;
}





SDF* SDF::create_difference(SDF* left_child, SDF* right_child)
{
    SDF* sdf = new SDF;

    sdf->m_left = left_child;
    sdf->m_right = right_child;
    sdf->m_type = SDF_DIFFERENCE;

    return sdf;
}

SDF* SDF::create_smooth_union(SDF* left_child, SDF* right_child, float smooth_radius)
{
    SDF* sdf = new SDF;

    sdf->m_left = left_child;
    sdf->m_right = right_child;
    sdf->m_smooth_sdf_coefficient = smooth_radius;
    sdf->m_type = SDF_SMOOTH_UNION;

    return sdf;
}

SDF* SDF::create_smooth_difference(SDF* left_child, SDF* right_child, float smooth_radius)
{
    SDF* sdf = new SDF;

    sdf->m_left = left_child;
    sdf->m_right = right_child;
    sdf->m_smooth_sdf_coefficient = smooth_radius;
    sdf->m_type = SDF_SMOOTH_DIFFERENCE;

    return sdf;
}

float SDF::Value(const Vector& point)
{
    switch (m_type)
    {
        case SDF_UNION:
            return std::min(m_left->Value(point), m_right->Value(point));

        case SDF_INTERSECTION:
            return std::max(m_left->Value(point), m_right->Value(point));

        case SDF_DIFFERENCE:
            return std::max(m_left->Value(point), -m_right->Value(point));

        case SDF_SMOOTH_UNION:
        {
            float value_left, value_right;
            value_left = m_left->Value(point);
            value_right = m_right->Value(point);

            return std::min(value_left, value_right) - smooth_min_polynomial(value_left, value_right, m_smooth_sdf_coefficient);
        }
        case SDF_SMOOTH_DIFFERENCE:
        {
            float value_left, value_right;
            value_left = m_left->Value(point);
            value_right = m_right->Value(point);

            return std::max(value_left, -value_right) + smooth_min_polynomial(value_left, -value_right, m_smooth_sdf_coefficient);
        }
        case SDF_SPHERE:
        {
            Point p_transformed = Point(point);

            if (m_transform_set)
                p_transformed = m_inverse_transform(Point(point.x, point.y, point.z));

            return length(point - m_sphere_center) - m_sphere_radius;
        }
        case SDF_TORUS:
        {
            Point p_transformed = Point(point);

            if (m_transform_set)
                p_transformed = m_inverse_transform(Point(point.x, point.y, point.z));

            return length(Vector(std::sqrt(p_transformed[0] * p_transformed[0] + p_transformed[2] * p_transformed[2]) - m_torus_inner_radius, p_transformed[1], 0) ) - m_torus_outer_radius;
        }
    }
}

float SDF::smooth_min_polynomial(float value_left, float value_right, float smooth_radius)
{
    float h = std::max(smooth_radius - std::abs(value_left - value_right), 0.0f) / smooth_radius;
    return 1.0f/6.0f * smooth_radius * h*h*h;
}
