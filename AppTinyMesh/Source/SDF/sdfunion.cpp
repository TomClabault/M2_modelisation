#include "sdfunion.h"

#include "SDF/SDF.h"

SDFUnion::SDFUnion(SDF* left, SDF* right) : m_left(left), m_right(right) {}

SDFUnion::~SDFUnion()
{
    delete m_left;
    delete m_right;
}

float SDFUnion::Value(const Vector& point)
{
    return std::min(m_left->Value(point), m_right->Value(point));
}
