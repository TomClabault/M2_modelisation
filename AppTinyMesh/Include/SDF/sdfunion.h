#ifndef SDFUNION_H
#define SDFUNION_H

#include "SDF/SDF.h"

class SDFUnion : public SDF
{
public:
    SDFUnion(SDF* right, SDF* left);
    virtual ~SDFUnion();

    virtual float Value(const Vector& point) override;

private:
    SDF* m_left;
    SDF* m_right;
};

#endif // SDFUNION_H
