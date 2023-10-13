#ifndef BEZIER_SURFACE_H
#define BEZIER_SURFACE_H

#include "beziersurface.h"

Mesh BezierSurface::polygonize(float step_u, float step_v)
{
    Mesh out_mesh;

    std::vector<Vector> normals;
    std::vector<int> normals_indices;
    std::vector<Vector> points;
    std::vector<int> triangles_indices;

    int nb_points_along_u = 1.0f / step_u + 1;
    int nb_points_along_v = 1.0f / step_v + 1;

    float u = 0.0f;
    for (int u_index = 0; u_index < nb_points_along_u; u_index++, u += step_u)
    {
        u = std::min(1.0f, u);

        std::vector<Point> curve_along_v_control_points;
        for (BezierCurve& curve : m_control_curves)
            curve_along_v_control_points.push_back(curve.evaluate(u));

        float v = 0.0f;
        for (int v_index = 0; v_index < nb_points_along_v; v_index++, v += step_v)
        {
            v = std::min(1.0f, v);
            BezierCurve curve_along_v(curve_along_v_control_points);
            Vector new_point = Vector(curve_along_v.evaluate(v));

            points.push_back(new_point);

            //We're going to start meshing the surface if we already have
            //a first row (along v) of points because every point that we're going
            //to add from now will be able to describe a triangle (or 2) on the surface
            if (u_index > 0)
            {
                if (v_index == 0)
                {
                    //Only one triangle to create for the first point

                    int index_0 = u_index * nb_points_along_v + v_index;
                    int index_1 = (u_index - 1) * nb_points_along_v + (v_index + 1);
                    int index_2 = (u_index - 1) * nb_points_along_v + v_index;

                    triangles_indices.push_back(index_0);
                    triangles_indices.push_back(index_1);
                    triangles_indices.push_back(index_2);

                    normals.push_back(normalize(cross(points[index_1] - points[index_0], points[index_2] - points[index_0])));
                    normals_indices.push_back(normals.size() - 1);
                    normals_indices.push_back(normals.size() - 1);
                    normals_indices.push_back(normals.size() - 1);
                }
                else if (v_index == nb_points_along_v - 1)
                {
                    //Only one triangle to create for the last point

                    int index_0 = u_index * nb_points_along_v + v_index;
                    int index_1 = (u_index - 1) * nb_points_along_v + v_index;
                    int index_2 = u_index * nb_points_along_v + (v_index - 1);

                    triangles_indices.push_back(index_0);
                    triangles_indices.push_back(index_1);
                    triangles_indices.push_back(index_2);

                    normals.push_back(normalize(cross(points[index_1] - points[index_0], points[index_2] - points[index_0])));
                    normals_indices.push_back(normals.size() - 1);
                    normals_indices.push_back(normals.size() - 1);
                    normals_indices.push_back(normals.size() - 1);
                }
                else
                {
                    int index_0 = u_index * nb_points_along_v + v_index;
                    int index_1 = (u_index - 1) * nb_points_along_v + v_index;
                    int index_2 = u_index * nb_points_along_v + (v_index - 1);
                    //Two triangles to create
                    triangles_indices.push_back(index_0);
                    triangles_indices.push_back(index_1);
                    triangles_indices.push_back(index_2);

                    normals.push_back(normalize(cross(points[index_1] - points[index_0], points[index_2] - points[index_0])));
                    normals_indices.push_back(normals.size() - 1);
                    normals_indices.push_back(normals.size() - 1);
                    normals_indices.push_back(normals.size() - 1);

                    index_0 = u_index * nb_points_along_v + v_index;
                    index_1 = (u_index - 1) * nb_points_along_v + (v_index + 1);
                    index_2 = (u_index - 1) * nb_points_along_v + v_index;

                    triangles_indices.push_back(index_0);
                    triangles_indices.push_back(index_1);
                    triangles_indices.push_back(index_2);

                    normals.push_back(normalize(cross(points[index_1] - points[index_0], points[index_2] - points[index_0])));
                    normals_indices.push_back(normals.size() - 1);
                    normals_indices.push_back(normals.size() - 1);
                    normals_indices.push_back(normals.size() - 1);
                }
            }
        }
    }

    return Mesh(points, normals, triangles_indices, normals_indices);
}

#endif
