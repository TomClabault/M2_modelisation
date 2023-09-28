#ifndef TORUSSDF_H
#define TORUSSDF_H

#include "mat.h"

#include "SDF/SDF.h"

class TorusSDF : public SDF
{
public:
    TorusSDF(float inner_radius, float outer_radius, Transform transformation = Identity());

    virtual float Value(const Vector &point) override;

private:
    Transform m_transform_inverse;

    float m_inner_radius;
    float m_outer_radius;
};

#endif // TORUSSDF_H
