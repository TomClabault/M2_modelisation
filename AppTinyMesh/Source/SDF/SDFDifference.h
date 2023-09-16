#ifndef SDFDIFFERENCE_H
#define SDFDIFFERENCE_H

#include "SDF/SDF.h"

class SDFDifference : public SDF
{
public:
    SDFDifference(SDF *left, SDF *right);
    virtual ~SDFDifference();

    virtual float Value(const Vector& point) override;

private:
    SDF* m_left;
    SDF* m_right;
};

#endif // SDFDIFFERENCE_H
