#ifndef SDFSMOOTHUNION_H
#define SDFSMOOTHUNION_H


#include "SDF/SDF.h"
class SDFSmoothUnion : public SDF
{
public:
    SDFSmoothUnion(SDF* left, SDF* right, float smooth_radius);
    virtual ~SDFSmoothUnion();

    float smooth_min_polynomial(float value_left, float value_right, float smooth_radius);
    virtual float Value(const Vector& point) override;

private:
    SDF* m_left;
    SDF* m_right;

    float m_smooth_radius;
};

#endif // SDFSMOOTHUNION_H
