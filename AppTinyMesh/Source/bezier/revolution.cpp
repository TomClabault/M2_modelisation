#include "revolution.h"

#include "mat.h"

Mesh Revolution::polygonize(int nb_steps, int nb_bezier_step, int rotation_axis)
{
    std::vector<Point> profile_points = m_profile.get_control_points();

    std::vector<Vector> out_points;
    std::vector<Vector> normals;
    std::vector<int> indices, normals_indices;

    for (int step = 0; step < nb_steps; step++)
    {
        float angle = 360.0f / nb_steps * step;//In degrees

        Transform rotation;
        switch (rotation_axis) {
        case 0:
            rotation = RotationX(angle);
            break;

        case 1:
            rotation = RotationY(angle);
            break;

        case 2:
            rotation = RotationZ(angle);
            break;

        default:
            rotation = RotationX(angle);
            break;
        }

        float u = 0.0f;
        float u_step = 1.0f / (nb_bezier_step + 1);
        for (int bezier_step = 0; bezier_step < nb_bezier_step; bezier_step++, u += u_step)
        {
            u = std::min(1.0f, u);

            out_points.push_back(Vector(rotation(m_profile.evaluate(u))));

            if (step > 0)
            {
                //If we already have our first line of points, we're going to start
                //meshing
                if (bezier_step == 0)
                {
                    //Only one triangle to create
                    int index_0 = step * nb_bezier_step + bezier_step;
                    int index_1 = (step - 1) * nb_bezier_step + (bezier_step + 1);
                    int index_2 = (step - 1) * nb_bezier_step + bezier_step;

                    indices.push_back(index_0);
                    indices.push_back(index_1);
                    indices.push_back(index_2);

                    normals_indices.push_back(normals.size());
                    normals_indices.push_back(normals.size());
                    normals_indices.push_back(normals.size());

                    normals.push_back(normalize(cross(out_points[index_1] - out_points[index_0], out_points[index_2] - out_points[index_0])));
                }
                else if (bezier_step == nb_bezier_step - 1)
                {
                    //Only one triangle to create
                    int index_0 = step * nb_bezier_step + bezier_step;
                    int index_1 = (step - 1) * nb_bezier_step + bezier_step;
                    int index_2 = step * nb_bezier_step + (bezier_step - 1);

                    indices.push_back(index_0);
                    indices.push_back(index_1);
                    indices.push_back(index_2);

                    normals_indices.push_back(normals.size());
                    normals_indices.push_back(normals.size());
                    normals_indices.push_back(normals.size());

                    normals.push_back(normalize(cross(out_points[index_1] - out_points[index_0], out_points[index_2] - out_points[index_0])));
                }
                else
                {
                    //Two triangles to create
                    int index_0 = step * nb_bezier_step + bezier_step;
                    int index_1 = (step - 1) * nb_bezier_step + bezier_step;
                    int index_2 = step * nb_bezier_step + (bezier_step - 1);
                    int index_3 = index_0;
                    int index_4 = (step - 1) * nb_bezier_step + (bezier_step + 1);
                    int index_5 = (step - 1) * nb_bezier_step + bezier_step;

                    indices.push_back(index_0);
                    indices.push_back(index_1);
                    indices.push_back(index_2);

                    indices.push_back(index_3);
                    indices.push_back(index_4);
                    indices.push_back(index_5);

                    normals_indices.push_back(normals.size());
                    normals_indices.push_back(normals.size());
                    normals_indices.push_back(normals.size());

                    normals.push_back(normalize(cross(out_points[index_1] - out_points[index_0], out_points[index_2] - out_points[index_0])));

                    normals_indices.push_back(normals.size());
                    normals_indices.push_back(normals.size());
                    normals_indices.push_back(normals.size());

                    normals.push_back(normalize(cross(out_points[index_4] - out_points[index_3], out_points[index_5] - out_points[index_3])));
                }
            }
        }
    }

    int last_row_start_index = (nb_steps - 1) * nb_bezier_step;
    //Last iteration of meshing to close the shape
    for (int i = 0; i < nb_bezier_step; i++)
    {
        if (i == 0)
        {
            //Only one triangle

            int index_0 = 0;
            int index_1 = last_row_start_index + 1;
            int index_2 = last_row_start_index;

            indices.push_back(index_0);
            indices.push_back(index_1);
            indices.push_back(index_2);

            normals_indices.push_back(normals.size());
            normals_indices.push_back(normals.size());
            normals_indices.push_back(normals.size());

            normals.push_back(normalize(cross(out_points[index_1] - out_points[index_0], out_points[index_2] - out_points[index_0])));
        }
        else if (i == nb_bezier_step - 1)
        {
            //Only one triangle

            int index_0 = i;
            int index_1 = last_row_start_index + i;
            int index_2 = i - 1;

            indices.push_back(index_0);
            indices.push_back(index_1);
            indices.push_back(index_2);

            normals_indices.push_back(normals.size());
            normals_indices.push_back(normals.size());
            normals_indices.push_back(normals.size());

            normals.push_back(normalize(cross(out_points[index_1] - out_points[index_0], out_points[index_2] - out_points[index_0])));
        }
        else
        {
            //Two triangles

            int index_0 = i;
            int index_1 = last_row_start_index + i;
            int index_2 = i - 1;

            indices.push_back(index_0);
            indices.push_back(index_1);
            indices.push_back(index_2);

            normals_indices.push_back(normals.size());
            normals_indices.push_back(normals.size());
            normals_indices.push_back(normals.size());

            normals.push_back(normalize(cross(out_points[index_1] - out_points[index_0], out_points[index_2] - out_points[index_0])));

            int index_3 = index_0;
            int index_4 = last_row_start_index + i + 1;
            int index_5 = last_row_start_index + i;

            indices.push_back(index_3);
            indices.push_back(index_4);
            indices.push_back(index_5);

            normals_indices.push_back(normals.size());
            normals_indices.push_back(normals.size());
            normals_indices.push_back(normals.size());

            normals.push_back(normalize(cross(out_points[index_4] - out_points[index_3], out_points[index_5] - out_points[index_3])));
        }
    }

    return Mesh(out_points, normals, indices, normals_indices);
}
