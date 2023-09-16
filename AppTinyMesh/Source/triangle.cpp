#include "triangle.h"

Vector Triangle::AreaNormal() const
{
    return 0.5 * ((p[1] - p[0]) / (p[2] - p[0]));
}
