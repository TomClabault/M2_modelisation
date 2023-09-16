#include "SDFDifference.h"

SDFDifference::SDFDifference(SDF* left, SDF* right) : m_left(left), m_right(right)  {}

SDFDifference::~SDFDifference()
{
    delete m_left;
    delete m_right;
}

float SDFDifference::Value(const Vector &point)
{
    return std::max(m_left->Value(point), -m_right->Value(point));
}
