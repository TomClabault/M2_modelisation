#ifndef REVOLUTION_H
#define REVOLUTION_H

#include "beziercurve.h"

#include <vector>

class Revolution
{
public:
    Revolution(BezierCurve& profile) : m_profile(profile) {}

    /**
     * @brief polygonize
     * @param nb_steps
     * @param rotation_axis 0, 1 or 2 for X, Y, Z respectively
     * @return
     */
    Mesh polygonize(int nb_steps, int nb_bezier_step, int rotation_axis);

private:
    BezierCurve m_profile;
};

#endif
