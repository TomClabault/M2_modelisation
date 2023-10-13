#include "beziercurve.h"

Point BezierCurve::evaluate(float u)
{
    std::vector<Point> current_points = m_control_points;
    while (current_points.size() >= 2)
    {
        std::vector<Point> new_points;
        new_points.reserve(current_points.size() - 1);

        for (int i = 0; i < current_points.size() - 1; i++)
        {
            Point& point_a = current_points[i];
            Point& point_b = current_points[i + 1];

            Point new_point = (1.0f - u) * point_a + point_b * u;

            new_points.push_back(new_point);
        }

        current_points = new_points;
    }

    return current_points[0];
}

std::vector<Point> BezierCurve::discretize(float step)
{
    float u = 0.0f;
    std::vector<Point> points;

    while (u <= 1.0f)
    {
        points.push_back(evaluate(u));

        u += step;
    }

    points.push_back(m_control_points.back());
    return points;
}
