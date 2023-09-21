#ifndef SDF_H
#define SDF_H

#include "vec.h"

#include <functional>


class SDF
{
public:
    virtual ~SDF() {};

    virtual float Value(const Vector& point) = 0;
};

#endif // SDF_H