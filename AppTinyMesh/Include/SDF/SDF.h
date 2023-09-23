#ifndef SDF_H
#define SDF_H

#include "vec.h"

#include <functional>

//TODO sans polymorphisme avec une Enum {SDFUnion, SDFIntersection, ...}

class SDF
{
public:
    virtual ~SDF() {};

    virtual float Value(const Vector& point) = 0;
};

#endif // SDF_H
