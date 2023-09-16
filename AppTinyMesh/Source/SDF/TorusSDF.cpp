#include "TorusSDF.h"

#include <cmath>

TorusSDF::TorusSDF(float inner_radius, float outer_radius, Transform transformation) : m_inner_radius(inner_radius), m_outer_radius(outer_radius),
                                                                                       m_transform_inverse(transformation.inverse()) { }

float TorusSDF::Value(const Vector& p)
{
    Point p_transformed = m_transform_inverse(Point(p.x, p.y, p.z));

    return length(Vector(std::sqrt(p_transformed[0] * p_transformed[0] + p_transformed[2] * p_transformed[2]) - m_inner_radius, p_transformed[1], 0) ) - m_outer_radius;
}
