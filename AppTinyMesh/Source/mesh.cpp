#include "mesh.h"

#include "mat.h"

/*!
\class Mesh mesh.h

\brief Core triangle mesh class.
*/



/*!
\brief Initialize the mesh to empty.
*/
Mesh::Mesh()
{
}

/*!
\brief Initialize the mesh from a list of vertices and a list of triangles.

Indices must have a size multiple of three (three for triangle vertices and three for triangle normals).

\param vertices List of geometry vertices.
\param indices List of indices wich represent the geometry triangles.
*/
Mesh::Mesh(const std::vector<Vector>& vertices, const std::vector<int>& indices) :m_vertices(vertices), m_varray(indices)
{
    m_normals.resize(vertices.size(), Vector::Z);
}

/*!
\brief Create the mesh.

\param vertices Array of vertices.
\param normals Array of normals.
\param va, na Array of vertex and normal indexes.
*/
Mesh::Mesh(const std::vector<Vector>& vertices, const std::vector<Vector>& normals, const std::vector<int>& va, const std::vector<int>& na) :m_vertices(vertices), m_normals(normals), m_varray(va), m_narray(na)
{
    m_triangles = new std::vector<Triangle>;
    m_triangles->reserve(va.size() / 3);
    for (int i = 0; i < va.size(); i += 3)
        m_triangles->push_back(Triangle(vertices[va[i]], vertices[va[i + 1]], vertices[va[i + 2]]));

    m_bvh = new BVH(m_triangles);
}

/*!
\brief Reserve memory for arrays.
\param nv,nn,nvi,nvn Number of vertices, normals, vertex indexes and vertex normals.
*/
void Mesh::Reserve(int nv, int nn, int nvi, int nvn)
{
    m_vertices.reserve(nv);
    m_normals.reserve(nn);
    m_varray.reserve(nvi);
    m_narray.reserve(nvn);
}

/*!
\brief Empty
*/
Mesh::~Mesh()
{
    //delete m_bvh;
}

void Mesh::set_corresponding_sdf(SDF *sdf)
{
    m_sdf = sdf;
}

void Mesh::twist(float rotation_extent, int twist_axis)
{
    for (Vector& vertex : m_vertices)
    {
        Transform rotation;
        float intensity = vertex[twist_axis] - (int)(vertex[twist_axis] / rotation_extent) * rotation_extent;
        float angle = 360.0f / rotation_extent * intensity;

        switch (twist_axis)
        {
        case 0:
            rotation = RotationX(angle);
            break;

        case 1:
            rotation = RotationY(angle);
            break;

        case 2:
            rotation = RotationZ(angle);
            break;
        }

        vertex = Vector(rotation(Point(vertex)));
    }
}

void Mesh::local_attenuated_translation(const int vertex_index_center_of_deformation, float deformation_radius, const Vector& translation)
{
    Vector center_vertex = m_vertices[vertex_index_center_of_deformation];

    float deformation_radius_squared = deformation_radius;// * deformation_radius;
    int debug_index = 0;
    for (Vector& vertex : m_vertices)
    {
        float distance_squared = Norm(vertex - center_vertex);

        //Attenuation is going to be 0 if we're outside of the deformation distance
        float x = 1.0f - std::min(1.0f, distance_squared / deformation_radius_squared);
        float strength = x * x * (3.0f - 2*x);

        vertex = vertex + translation * strength;

        debug_index++;
    }
}

Vector Mesh::intersect(const Ray& ray) const
{
    HitInfo hit_info;
    Vector inter_point;
    if(m_bvh->intersect(ray, hit_info))
        inter_point = ray.Origin() + hit_info.t * ray.Direction();
    else
        inter_point = Vector(0, 0, 0);

    return inter_point;
}

Vector Mesh::intersectRayMarching(SDF* sdf, const Ray &ray, const float step_size, const int max_steps, const float max_distance_away) const
{
    int current_step = 0;
    float current_distance_to_sdf = std::numeric_limits<float>::max();
    float previous_distance_to_sdf = current_distance_to_sdf;
    Vector current_position = ray.Origin();
    Vector previous_position = current_position;
    while ((current_step++ < max_steps || max_steps == -1)
        && (current_distance_to_sdf < max_distance_away || current_distance_to_sdf == std::numeric_limits<float>::max()))
    {
        current_distance_to_sdf = sdf->Value(current_position);

        //If we found the almost exact intersection, directly returning
        if (std::abs(current_distance_to_sdf) < 1.0e-5f)
            return current_position;

        current_position += ray.Direction() * step_size;

        //We changed sign, this means that we went through the surface
        if (current_distance_to_sdf * previous_distance_to_sdf < 0)
            break;

        previous_distance_to_sdf = current_distance_to_sdf;
        previous_position = current_position;
    }

    //No intersection found
    if (current_step == max_steps + 1)
        return Vector(0, 0, 0);
    else
        return (previous_position + current_position) / 2;
}

/*!
\brief Smooth the normals of the mesh.

This function weights the normals of the faces by their corresponding area.
\sa Triangle::AreaNormal()
*/
void Mesh::SmoothNormals()
{
    // Initialize
    m_normals.resize(m_vertices.size(), Vector::Null);

    m_narray = m_varray;

    // Accumulate normals
    for (int i = 0; i < m_varray.size(); i += 3)
    {
        Vector tn = Triangle(m_vertices[m_varray.at(i)], m_vertices[m_varray.at(i + 1)], m_vertices[m_varray.at(i + 2)]).AreaNormal();
        m_normals[m_narray[i + 0]] += tn;
        m_normals[m_narray[i + 1]] += tn;
        m_normals[m_narray[i + 2]] += tn;
    }

    // Normalize
    for (int i = 0; i < m_normals.size(); i++)
    {
        Normalize(m_normals[i]);
    }
}

/*!
\brief Add a smooth triangle to the geometry.
\param a, b, c Index of the vertices.
\param na, nb, nc Index of the normals.
*/
void Mesh::AddSmoothTriangle(int a, int na, int b, int nb, int c, int nc)
{
    m_varray.push_back(a);
    m_narray.push_back(na);
    m_varray.push_back(b);
    m_narray.push_back(nb);
    m_varray.push_back(c);
    m_narray.push_back(nc);
}

/*!
\brief Add a triangle to the geometry.
\param a, b, c Index of the vertices.
\param n Index of the normal.
*/
void Mesh::AddTriangle(int a, int b, int c, int n)
{
    m_varray.push_back(a);
    m_narray.push_back(n);
    m_varray.push_back(b);
    m_narray.push_back(n);
    m_varray.push_back(c);
    m_narray.push_back(n);
}

/*!
\brief Add a smmoth quadrangle to the geometry.

Creates two smooth triangles abc and acd.

\param a, b, c, d  Index of the vertices.
\param na, nb, nc, nd Index of the normal for all vertices.
*/
void Mesh::AddSmoothQuadrangle(int a, int na, int b, int nb, int c, int nc, int d, int nd)
{
    // First triangle
    AddSmoothTriangle(a, na, b, nb, c, nc);

    // Second triangle
    AddSmoothTriangle(a, na, c, nc, d, nd);
}

/*!
\brief Add a quadrangle to the geometry.

\param a, b, c, d  Index of the vertices and normals.
*/
void Mesh::AddQuadrangle(int a, int b, int c, int d)
{
    AddSmoothQuadrangle(a, a, b, b, c, c, d, d);
}

/*!
\brief Compute the bounding box of the object.
*/
Box Mesh::GetBox() const
{
    if (m_vertices.size() == 0)
    {
        return Box::Null;
    }
    return Box(m_vertices);
}

/*!
\brief Creates an axis aligned box.

The object has 8 vertices, 6 normals and 12 triangles.
\param box The box.
*/
Mesh::Mesh(const Box& box)
{
    // Vertices
    m_vertices.resize(8);

    for (int i = 0; i < 8; i++)
    {
        m_vertices[i] = box.Vertex(i);
    }

    // Normals
    m_normals.push_back(Vector(-1, 0, 0));
    m_normals.push_back(Vector(1, 0, 0));
    m_normals.push_back(Vector(0, -1, 0));
    m_normals.push_back(Vector(0, 1, 0));
    m_normals.push_back(Vector(0, 0, -1));
    m_normals.push_back(Vector(0, 0, 1));

    // Reserve space for the triangle array
    m_varray.reserve(12 * 3);
    m_narray.reserve(12 * 3);

    AddTriangle(0, 2, 1, 4);
    AddTriangle(1, 2, 3, 4);

    AddTriangle(4, 5, 6, 5);
    AddTriangle(5, 7, 6, 5);

    AddTriangle(0, 4, 2, 0);
    AddTriangle(4, 6, 2, 0);

    AddTriangle(1, 3, 5, 1);
    AddTriangle(3, 7, 5, 1);

    AddTriangle(0, 1, 5, 2);
    AddTriangle(0, 5, 4, 2);

    AddTriangle(3, 2, 7, 3);
    AddTriangle(6, 7, 2, 3);
}

/*!
\brief Scale the mesh.
\param s Scaling factor.
*/
void Mesh::Scale(double s)
{
    // Vertexes
    for (int i = 0; i < m_vertices.size(); i++)
    {
        m_vertices[i] *= s;
    }

    if (s < 0.0)
    {
        // Normals
        for (int i = 0; i < m_normals.size(); i++)
        {
            m_normals[i] = -m_normals[i];
        }
    }
}



#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QRegularExpression>
#include <QtCore/qstring.h>

/*!
\brief Import a mesh from an .obj file.
\param filename File name.
*/
void Mesh::Load(const QString& filename)
{
    m_vertices.clear();
    m_normals.clear();
    m_varray.clear();
    m_narray.clear();

    QFile data(filename);

    if (!data.open(QFile::ReadOnly))
        return;
    QTextStream in(&data);

    // Set of regular expressions : Vertex, Normal, Triangle
    QRegularExpression rexv("v\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)");
    QRegularExpression rexn("vn\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)");
    QRegularExpression rext("f\\s*(\\d*)/\\d*/(\\d*)\\s*(\\d*)/\\d*/(\\d*)\\s*(\\d*)/\\d*/(\\d*)");
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QRegularExpressionMatch match = rexv.match(line);
        QRegularExpressionMatch matchN = rexn.match(line);
        QRegularExpressionMatch matchT = rext.match(line);
        if (match.hasMatch())//rexv.indexIn(line, 0) > -1)
        {
            Vector q = Vector(match.captured(1).toDouble(), match.captured(2).toDouble(), match.captured(3).toDouble()); m_vertices.push_back(q);
        }
        else if (matchN.hasMatch())//rexn.indexIn(line, 0) > -1)
        {
            Vector q = Vector(matchN.captured(1).toDouble(), matchN.captured(2).toDouble(), matchN.captured(3).toDouble());  m_normals.push_back(q);
        }
        else if (matchT.hasMatch())//rext.indexIn(line, 0) > -1)
        {
            m_varray.push_back(matchT.captured(1).toInt() - 1);
            m_varray.push_back(matchT.captured(3).toInt() - 1);
            m_varray.push_back(matchT.captured(5).toInt() - 1);
            m_narray.push_back(matchT.captured(2).toInt() - 1);
            m_narray.push_back(matchT.captured(4).toInt() - 1);
            m_narray.push_back(matchT.captured(6).toInt() - 1);
        }
    }
    data.close();
}

/*!
\brief Save the mesh in .obj format, with vertices and normals.
\param url Filename.
\param meshName %Mesh name in .obj file.
*/
void Mesh::SaveObj(const QString& url, const QString& meshName) const
{
    QFile data(url);
    if (!data.open(QFile::WriteOnly))
        return;
    QTextStream out(&data);
    out << "g " << meshName << Qt::endl;
    for (int i = 0; i < m_vertices.size(); i++)
        out << "v " << m_vertices.at(i)[0] << " " << m_vertices.at(i)[1] << " " << m_vertices.at(i)[2] << QString('\n');
    for (int i = 0; i < m_normals.size(); i++)
        out << "vn " << m_normals.at(i)[0] << " " << m_normals.at(i)[1] << " " << m_normals.at(i)[2] << QString('\n');
    for (int i = 0; i < m_varray.size(); i += 3)
    {
        out << "f " << m_varray.at(i) + 1 << "//" << m_narray.at(i) + 1 << " "
            << m_varray.at(i + 1) + 1 << "//" << m_narray.at(i + 1) + 1 << " "
            << m_varray.at(i + 2) + 1 << "//" << m_narray.at(i + 2) + 1 << " "
            << "\n";
    }
    out.flush();
    data.close();
}

