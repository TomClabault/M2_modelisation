#include "SDFSmoothUnion.h"

SDFSmoothUnion::SDFSmoothUnion(SDF* left, SDF* right, float smooth_radius) : m_left(left), m_right(right), m_smooth_radius(smooth_radius) { }

SDFSmoothUnion::~SDFSmoothUnion()
{
    delete m_left;
    delete m_right;
}

float SDFSmoothUnion::smooth_min_polynomial(float value_left, float value_right, float smooth_radius)
{
    float h = std::max(smooth_radius - std::abs(value_left - value_right), 0.0f) / smooth_radius;
    return 1.0f/6.0f * smooth_radius * h*h*h;
}

float SDFSmoothUnion::Value(const Vector &point)
{
    float value_left, value_right;
    value_left = m_left->Value(point);
    value_right = m_right->Value(point);

    return std::min(value_left, value_right) - smooth_min_polynomial(value_left, value_right, m_smooth_radius);
}
