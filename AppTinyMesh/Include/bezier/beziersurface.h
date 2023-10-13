#include "beziercurve.h"

#include <vector>

class BezierSurface
{
public:
    BezierSurface(std::vector<BezierCurve>& control_curves) : m_control_curves(control_curves) {}

    Mesh polygonize(float step_u, float step_v);

private:
    std::vector<BezierCurve> m_control_curves;
};
