#ifndef SDFINTERSECTION_H
#define SDFINTERSECTION_H

#include "mathematics.h"
#include "SDF/SDF.h"

class SDFIntersection : public SDF
{
public:
    SDFIntersection(SDF* right, SDF* left);
    virtual ~SDFIntersection();

    virtual float Value(const Vector& point) override;

private:
    SDF* m_right;
    SDF* m_left;
};

#endif // SDFINTERSECTION_H
