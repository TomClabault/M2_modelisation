#ifndef BEZIER_CURVE_H
#define BEZIER_CURVE_H

#include "mesh.h"
#include "vec.h"

#include <vector>

class BezierCurve
{
public:
    BezierCurve(std::vector<Point>& control_points) : m_control_points(control_points) {}

    Point evaluate(float u);
    std::vector<Point> discretize(float step);

    const std::vector<Point>& get_control_points() const;

private:
    std::vector<Point> m_control_points;
};

#endif
