#ifndef SDFSMOOTHDIFFERENCE_H
#define SDFSMOOTHDIFFERENCE_H


#include "SDF/SDF.h"
class SDFSmoothDifference : public SDF
{
public:
    SDFSmoothDifference(SDF *left, SDF *right);

    virtual float Value(const Vector& point) override;

private:
    SDF* m_left;
    SDF* m_right;
};

#endif // SDFSMOOTHDIFFERENCE_H
