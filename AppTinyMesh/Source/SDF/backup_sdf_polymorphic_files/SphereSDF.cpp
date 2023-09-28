#include "SphereSDF.h"

SphereSDF::SphereSDF(const Vector& center, float radius) : m_center(center), m_radius(radius) { }

float SphereSDF::Value(const Vector& point)
{
    return length(point - m_center) - m_radius;
}
