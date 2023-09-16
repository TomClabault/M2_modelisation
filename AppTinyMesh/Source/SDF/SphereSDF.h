#ifndef SPHERESDF_H
#define SPHERESDF_H

#include "mathematics.h"
#include "SDF/SDF.h"

class SphereSDF : public SDF
{
public:
    SphereSDF(const Vector& center, float radius);

    virtual float Value(const Vector& point) override;

private:
    Vector m_center;
    float m_radius;
};

#endif // SPHERESDF_H
