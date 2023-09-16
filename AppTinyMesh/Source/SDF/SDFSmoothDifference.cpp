#include "SDFSmoothDifference.h"

SDFSmoothDifference::SDFSmoothDifference(SDF* left, SDF* right) : m_left(left), m_right(right) {}

float smooth_min_polynomial(float value_left, float value_right, float smooth_radius)
{
    float h = std::max(smooth_radius - std::abs(value_left - value_right), 0.0f) / smooth_radius;
    return 1.0f/6.0f * smooth_radius * h*h*h;
}

float SDFSmoothDifference::Value(const Vector &point)
{
    float value_left, value_right;
    value_left = m_left->Value(point);
    value_right = m_right->Value(point);

    return std::max(value_left, -value_right) + smooth_min_polynomial(value_left, value_right, 0.3f);
}
