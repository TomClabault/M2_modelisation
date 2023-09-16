#include "SDFIntersection.h"

SDFIntersection::SDFIntersection(SDF* left, SDF* right) : m_right(right), m_left(left) { }

SDFIntersection::~SDFIntersection()
{
    delete m_left;
    delete m_right;
}

float SDFIntersection::Value(const Vector& point)
{
    return std::max(m_left->Value(point), m_right->Value(point));
}
